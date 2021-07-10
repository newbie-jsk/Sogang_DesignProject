#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace std;
using namespace cv;

enum interpolation
{
	NEAREST = 0,
	BILINEAR = 1,
	BICUBIC = 2,
};

enum t_thres
{
	BASIC = 0,
	OTSU = 1,
	USER = 2
};

Mat aff_transform(Mat src, Mat aff_mat, interpolation interp);
Mat resize(Mat src, int x, int y);
Mat mat_mult(Mat src, Mat kern);
Mat mat_mult_comp(Mat src1_fft, Mat src2_fft);
Mat scale(Mat src, float scale, interpolation interp);
Mat rotate(Mat src, float rad, interpolation interp);
Mat translate(Mat src, int x, int y, interpolation interp);
Mat threshold_my(Mat src, t_thres type);
Mat img_padding(Mat src, Size size);
Mat freqGaussFilter(Mat src, Size kernel_size);
Mat freqFilter(Mat src, Mat kernel);
Mat freqBoxFilter(Mat src, Size kernel_size);
Mat sobelFilter(Mat src, float direction);
Mat ma_threshold(Mat src, int n, float c);
Mat laplacianFilter(Mat src);
Mat percent_threshold(Mat src, float percentile);
Mat edge_threshold(Mat src, float percent, int mode);


float nearest(Mat src, float x, float y);
float bilinear(Mat src, float x, float y);
float bicubic(Mat src, float x, float y);
float cubic(float y0, float y1, float y2, float y3, float dx);
void getHist(Mat src, float hist[]);
uchar basic_threshold(float* hist);
uchar otsu_threshold(float* hist);
Mat multi_threshold(Mat src);
float sig_b(float* hist, uchar k1, uchar k2);
void histPrint(float* hist);

// dft.cpp
Mat fft2d(Mat src, int flags = DFT_COMPLEX_OUTPUT);
void fft2d(Mat src, Mat& dst, int flags = DFT_COMPLEX_OUTPUT);
Mat ifft2d(Mat src, int flags = DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
void ifft2d(Mat src, Mat& dst, int flags = DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);

Mat fftshift2d(Mat src);
void fftshift2d(Mat src, Mat& dst);
Mat ifftshift2d(Mat src);
void ifftshift2d(Mat src, Mat& dst);