#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdio> 
#include <string> 
#include <opencv2/opencv.hpp> 
#include <cmath> 
#include <iomanip>
#include <chrono>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <omp.h>
/* 
Serially compare each pixel by computing pixel difference to generate spectrogram of channel 
*/
unsigned char * serialCompare(const unsigned char* pre, const unsigned char* post, size_t numPixels) {
    unsigned char * chnl = (unsigned char *)malloc(numPixels*4*sizeof(unsigned char));
    for (size_t i = 0; i < numPixels; ++i) {  // 4 channels per pixel (RGBA)
        size_t idx = i*4;
        chnl[idx] = post[idx] - pre[idx]; // R
        chnl[idx+1] = post[idx+1] - pre[idx+1]; // G
        chnl[idx+2] = post[idx+2] - pre[idx+2]; // B
        chnl[idx+3] = post[idx+3] - pre[idx+3]; // A
    }
    return chnl;
}

/* 
Parallelized computation of pixel difference to generate spectrogram of channel (OpenMP) --> Precious
*/
unsigned char * parallelCompare(const unsigned char* pre, const unsigned char* post, size_t numPixels) {
    
    unsigned char * chnl = (unsigned char *)malloc(numPixels*4*sizeof(unsigned char));
    
    #pragma parallel
    {
        /*
        //Probably need to change indexing such that index corresponds to ID of thread
        int p = omp_get_thread_num();
        int nt = omp_get_numd_threads();
        //Number of threads is CPU based so calulate how many pixels each thread will have to do        
        */
        #pragma omp for
        for (size_t i = 0; i < numPixels; ++i) {  // 4 channels per pixel (RGBA)
        size_t idx = i*4;
        chnl[idx] = post[idx] - pre[idx]; // R
        chnl[idx+1] = post[idx+1] - pre[idx+1]; // G
        chnl[idx+2] = post[idx+2] - pre[idx+2]; // B
        chnl[idx+3] = post[idx+3] - pre[idx+3]; // A
        }
    }
    
    return chnl;
}

/* 
Vectorized computation of pixel difference to generate spectrogram of channel (AVX512 registers) --> Sam
*/
unsigned char * vectorizedCompare(const unsigned char* pre, const unsigned char* post, size_t numPixels) {
    unsigned char * chnl = (unsigned char *)malloc(numPixels*4*sizeof(unsigned char));
    
    return chnl;
}

/* Verify correctness across approaches */
bool diffCheck(const unsigned char * ch1, const unsigned char * ch2) {
    bool indicator;   
    //unsigned char * chnl = (unsigned char *)malloc(numPixels*4*sizeof(unsigned char));
    // size_t diffCount = 0;

    // for (size_t i = 0; i < numPixels; ++i) {
    //     size_t idx = i * 4;  // 4 channels per pixel (RGBA)
    //     if (img1[idx + 0] != img2[idx + 0] ||  // R
    //         img1[idx + 1] != img2[idx + 1] ||  // G
    //         img1[idx + 2] != img2[idx + 2] ||  // B
    //         img1[idx + 3] != img2[idx + 3]) {  // A
    //         ++diffCount;
    //     }
    // }
    
    // if (diffCount == 0) {
    // std::cout << "Images are the same\n";   
    // } else {
    // std::cout << "Images are not the same.\n Number of differing pixels: " << diffCount << std::endl;
    // }
    
    return false;
}

/*
Kernel that creates channel image for human verification
*/
void createChannelImage(unsigned char *in, const int rows, const int cols, std::string name)
{
    cv::Mat output(rows, cols, CV_8UC4, in); // generate image
    bool suc = cv::imwrite(name.c_str(), output);
    if(!suc){
        std::cerr << "Couldn't write Channel image!\n";
        exit(1);
    }
}


int main(int argc, char const *argv[]) {
    
    int im_rows, im_cols, cmp_type;
    std::string chnl_type;

    switch(argc){
        case 1:
            im_rows = 256;
            im_cols = 256;
            cmp_type = 0;
            chnl_type = "AWGN";
            break; 
        case 5:
            im_rows = atoi(argv[1]);
            im_cols = atoi(argv[2]);
            cmp_type = atoi(argv[3]);
            chnl_type = argv[4];
            break;

        default: 
                std::cerr << "Usage: rows columns compare_type channel_type\nValid compare types: \nserial=0 \nparallel=1 \nvectorized=2\n";
                exit(1);
    }
    
    const size_t  numPixels = im_rows*im_cols;
    std::cout << "Rows: " << im_rows << std::endl;
    std::cout << "Cols: " << im_cols << std::endl;
    std::cout << "Channel Type: " << chnl_type << std::endl;
    std::cout << "Size of channel image: " << numPixels*4*sizeof(unsigned char) << std::endl;
    
    //Read in image at specified file path
    std::ostringstream oss;
    oss << im_rows << "x" << im_cols << "-" << chnl_type;
    std::string folder_path = oss.str();
    std::string pre_path = folder_path + "/spectrograms/spectrogram_pre_0.bmp"; // Change "0" to test different spectrograms
    std::string post_path = folder_path + "/spectrograms/spectrogram_post_0.bmp"; // Change "0" to test different spectrograms
    
    cv::Mat pre = cv::imread(pre_path.c_str(), cv::IMREAD_COLOR);
    cv::Mat post = cv::imread(post_path.c_str(), cv::IMREAD_COLOR);
    
    cv::Mat pre_rgba, post_rgba; 
    cv::cvtColor(pre, pre_rgba, cv::COLOR_BGR2RGBA);
    cv::cvtColor(post, post_rgba, cv::COLOR_BGR2RGBA);
    
    const unsigned char* ptr1 = reinterpret_cast<const unsigned char*>(pre_rgba.data);
    const unsigned char* ptr2 = reinterpret_cast<const unsigned char*>(post_rgba.data);
    
    unsigned char* channel;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_time;
    
    //Does comparison based on type specifed by user
    if (cmp_type == 0) {
        start = std::chrono::system_clock::now();
        channel = serialCompare(ptr1, ptr2, numPixels);
        end = std::chrono::system_clock::now();
    }else if (cmp_type == 1){
        start = std::chrono::system_clock::now();
        channel = parallelCompare(ptr1, ptr2, numPixels);
        end = std::chrono::system_clock::now();
    }else if (cmp_type  == 1){
        start = std::chrono::system_clock::now();
        channel = vectorizedCompare(ptr1, ptr2, numPixels);
        end = std::chrono::system_clock::now();
    }else {
        std::cerr << "Invalid input for compare type \nValid compare types: \nserial=0 \nparallel=1 \nvectorized=2\n"; 
    }
    
    elapsed_time = end-start;
    std::cout << "Time: " << elapsed_time.count() << "s\n";
    
    std::string channel_name = folder_path + "-channel.bmp";
    createChannelImage(channel, im_rows, im_cols, channel_name); 
    
    return 0;
}