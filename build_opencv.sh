#!/bin/bash
#In the terminal run the following commands to build openCV:
#chmod +x build_opencv.sh
#./build_opencv.sh
#Put the following line in your bash profile or run if before running the program
#export LD_LIBRARY_PATH=$HOME/opencv/install/lib64:$LD_LIBRARY_PATH

INSTALL_DIR=$HOME/opencv/install
SRC_DIR=$HOME/opencv
BUILD_DIR=$HOME/opencv/build

# Clone if needed
if [ ! -d "$SRC_DIR" ]; then
    git clone https://github.com/opencv/opencv.git --branch 4.9.0 --depth 1 $SRC_DIR
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
  \
  -DBUILD_opencv_python2=OFF \
  -DBUILD_opencv_python3=OFF \
  -DBUILD_opencv_java=OFF \
  -DBUILD_opencv_js=OFF \
  -DBUILD_opencv_objc=OFF \
  \
  -DBUILD_opencv_video=OFF \
  -DBUILD_opencv_videoio=OFF \
  -DBUILD_opencv_highgui=OFF \
  -DBUILD_opencv_ml=OFF \
  -DBUILD_opencv_photo=OFF \
  -DBUILD_opencv_calib3d=OFF \
  -DBUILD_opencv_dnn=OFF \
  -DBUILD_opencv_features2d=OFF \
  -DBUILD_opencv_flann=OFF \
  -DBUILD_opencv_stitching=OFF \
  -DBUILD_opencv_superres=OFF \
  -DBUILD_opencv_videostab=OFF \
  \
  -DBUILD_JPEG=OFF \
  -DBUILD_PNG=OFF \
  -DBUILD_TIFF=OFF \
  -DBUILD_WEBP=OFF \
  -DBUILD_OPENEXR=OFF \
  \
  -DWITH_JPEG=OFF \
  -DWITH_PNG=OFF \
  -DWITH_TIFF=OFF \
  -DWITH_WEBP=OFF \
  -DWITH_OPENEXR=OFF \
  \
  -DWITH_IPP=OFF \
  -DWITH_TBB=OFF \
  -DWITH_OPENMP=OFF \
  -DWITH_FFMPEG=OFF \
  -DWITH_GSTREAMER=OFF \
  -DWITH_CUDA=OFF \
  -DWITH_GTK=OFF \
  -DWITH_QT=OFF \
  -DWITH_V4L=OFF \
  \
  $SRC_DIR

make -j 8 || exit 1
make install

