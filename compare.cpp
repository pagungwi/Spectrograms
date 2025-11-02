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

void serialCompare(const unsigned char* img1, const unsigned char* img2, size_t numPixels) {
    size_t diffCount = 0;

    for (size_t i = 0; i < numPixels; ++i) {
        size_t idx = i * 4;  // 4 channels per pixel (RGBA)
        if (img1[idx + 0] != img2[idx + 0] ||  // R
            img1[idx + 1] != img2[idx + 1] ||  // G
            img1[idx + 2] != img2[idx + 2] ||  // B
            img1[idx + 3] != img2[idx + 3]) {  // A
            ++diffCount;
        }
    }
    
    if (diffCount == 0) {
    std::cout << "Images are the same\n";   
    } else {
    std::cout << "Images are not the same.\n Number of differing pixels: " << diffCount << std::endl;
    }
}


int main(int argc, char const *argv[]) {
    
    int im_rows, im_cols, cmp_type;

    switch(argc){
        case 1:
            im_rows = 256;
            im_cols = 512;
            cmp_type = 0;
            break; 
        case 2:
            im_rows = atoi(argv[1]);
            im_cols = atoi(argv[2]);
            cmp_type = atoi(argv[3]);
            break;

        default: 
                std::cerr << "Usage: rows columns compare_type \nValid compare types: \nserial=0 \nparallel=1 \nvectorized=2\n";
                exit(1);
    }
    
    const size_t  numPixels = im_rows*im_cols;
    
    //Read in image - adapted from HW1 of GPU
    std::string image_1 = "256x512-AWGN/spectrograms/spectrogram_post_0.bmp"; 
    std::string image_2 = "256x512-AWGN/spectrograms/spectrogram_post_0.bmp"; 
    
    cv::Mat img1 = cv::imread(image_1.c_str(), cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread(image_2.c_str(), cv::IMREAD_COLOR);
    
    cv::Mat im1_rgba, im2_rgba; 
    cv::cvtColor(img1, im1_rgba, cv::COLOR_BGR2RGBA);
    cv::cvtColor(img2, im2_rgba, cv::COLOR_BGR2RGBA);
    
    const unsigned char* ptr1 = reinterpret_cast<const unsigned char*>(im1_rgba.data);
    const unsigned char* ptr2 = reinterpret_cast<const unsigned char*>(im2_rgba.data);
    
    //Does comparison based on type specifed by user
    if (cmp_type == 0) {
        serialCompare(ptr1, ptr2, numPixels);
    }else if (cmp_type == 1){
        //parallelCompare();
    }else if (cmp_type  == 1){
        //vectorizedCompare(); //We don't have to do this, I just figured it would be good to include something from class
    }else {
        std::cerr << "Invalid input for compare type \nValid compare types: \nserial=0 \nparallel=1 \nvectorized=2\n"; 
    }    
        
    
    return 0;
    }