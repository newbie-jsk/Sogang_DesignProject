#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace std;
using namespace cv;

enum FILT_TYPE {
	FILT_LOW = 0,
	FILT_HIGH = 1
};

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

// dft.cpp
Mat fft2d(Mat src, int flags = DFT_COMPLEX_OUTPUT);
void fft2d(Mat src, Mat& dst, int flags = DFT_COMPLEX_OUTPUT);
Mat ifft2d(Mat src, int flags = DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
void ifft2d(Mat src, Mat& dst, int flags = DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);

Mat fftshift2d(Mat src);
void fftshift2d(Mat src, Mat& dst);
Mat ifftshift2d(Mat src);
void ifftshift2d(Mat src, Mat& dst);

//filtering
Mat filtering(Mat src, Mat kern);
Mat get_gaussLPF(Size size, float sigma);
Mat get_idealLPF(Size size, float distance);
Mat get_idealboxLPF(Size size, Size kern_size);
Mat get_butterworthLPF(Size size, int order, float distance);
Mat idealfilter(Mat src, int distance, FILT_TYPE type = FILT_LOW);
Mat idealboxfilter(Mat src, Size size, FILT_TYPE type = FILT_LOW);
Mat gaussfilter(Mat src, float sigma, FILT_TYPE type = FILT_LOW);
Mat butterfilter(Mat src, int order, float distance, FILT_TYPE type = FILT_LOW);

Mat complex_mult(Mat src1_fft, Mat src2_fft);
void fft_show(Mat src);
Mat myshift(Mat src);

//Morphology
Mat erosion(Mat src, Mat se, Size se_center);
Mat dilation(Mat src, Mat se, Size se_center);
Mat opening(Mat src, Mat se, Size se_center);
Mat closing(Mat src, Mat se, Size se_center);
Mat img_complement(Mat src);
Mat img_sub(Mat src1, Mat src2);
Mat img_add(Mat src1, Mat src2);

//Threshold
void getHist(Mat src, float hist[]);
uchar basic_threshold(float* hist);
uchar otsu_threshold(float* hist);
Mat multi_threshold(Mat src);
float sig_b(float* hist, uchar k1, uchar k2);
void histPrint(float* hist);
Mat threshold_my(Mat src, t_thres type);
Mat ma_threshold(Mat src, int n, float c);
Mat percent_threshold(Mat src, float percentile);
Mat edge_threshold(Mat src, float percent, int mode);
void getHist(Mat src, float hist[]);
uchar basic_threshold(float* hist);
uchar otsu_threshold(float* hist);
Mat multi_threshold(Mat src);
float sig_b(float* hist, uchar k1, uchar k2);
void histPrint(float* hist);