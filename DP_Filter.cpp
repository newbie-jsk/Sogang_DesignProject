#include "DP_Work1.h"

Mat freqFilter(Mat src, Mat kernel)
{
	// Do Fast Fourier Transform
	Mat kern_fft = fftshift2d(fft2d(kernel));
	Mat src_fft = fftshift2d(fft2d(src));

	//	fft_printer(kern_fft, 0);
	//	fft_printer(src_fft, 0);

		// Source * Kernel
	Mat dst_fft = mat_mult_comp(src_fft, kern_fft);

	//	fft_printer(dst_fft, 0);

	// Inverse FFT
	Mat dst_ifft = ifft2d(fftshift2d(dst_fft));
	Mat dst;
	dst_ifft.convertTo(dst, CV_8U, 1, 0);

	return dst;
}

Mat freqGaussFilter(Mat src, Size kernel_size)
{
	//Declare variables
	Mat kernel(kernel_size, CV_32F, Scalar(0));
	float kernel_pixel_value, kernel_cummulative_value = 0;
	int row_center, col_center;

	//Make Gauss filter in spartial domain
	row_center = ceil(kernel_size.height / 2);
	col_center = ceil(kernel_size.width / 2);
	int sigma;
	if (kernel_size.width == 31)
		sigma = 5;
	else
		sigma = 9;

	for (int i = 0; i < kernel_size.height; i++)
	{
		float x_sqr = (i - row_center) * (i - row_center);
		for (int j = 0; j < kernel_size.width; j++)
		{
			float y_sqr = (j - col_center) * (j - col_center);

			// exp( - (x^2 + y^2) / 2 sig^2)
			kernel_pixel_value = exp(-1.0 * (x_sqr + y_sqr) / (2 * sigma * sigma));
			kernel.at<float>(i, j) = kernel_pixel_value;
			kernel_cummulative_value += kernel_pixel_value;
		}
	}

	//divide kernel by cummulated value
	kernel = kernel.mul(1.0 / kernel_cummulative_value);
	Mat padded_kern = img_padding(kernel, Size(1000, 1000));
	Mat padded_src = img_padding(src, Size(1000, 1000));

	//filtering
	Mat dst_prem = freqFilter(padded_src, padded_kern);

	Mat dst(src.size(), src.type());
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<uchar>(i, j) = dst_prem.at<uchar>(i, j);
		}
	}

	return dst;
}

Mat img_padding(Mat src, Size size)
{
	Mat dst(size, src.type(), Scalar(0));

	if (src.type() == CV_8U)
	{
		for (int j = 0; j < src.rows; j++)
		{
			for (int i = 0; i < src.cols; i++)
			{
				dst.at<uchar>(j, i) = src.at<uchar>(j, i);
			}
		}
	}
	else if (src.type() == CV_32F)
	{
		for (int j = 0; j < src.rows; j++)
		{
			for (int i = 0; i < src.cols; i++)
			{
				dst.at<float>(j, i) = src.at<float>(j, i);
			}
		}
	}


	return dst;
}

Mat freqBoxFilter(Mat src, Size kernel_size)
{
	//Make box kernel
	Mat kernel(kernel_size, src.type(), Scalar(1));

	//Padding two images
	Mat padded_src = img_padding(src, Size(1000, 1000));
	Mat padded_kern = img_padding(kernel, Size(1000, 1000));

	// box kernel * 1/kernel_size
	padded_kern.convertTo(padded_kern, CV_32F);
	float div = 1.0 / (kernel_size.width * kernel_size.height);
	padded_kern = padded_kern.mul(div);

	//filtering
	Mat dst_prem = freqFilter(padded_src, padded_kern);

	//Cut image to original size
	Mat dst(src.size(), src.type());
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<uchar>(i, j) = dst_prem.at<uchar>(i, j);
		}
	}

	return dst;
}

Mat sobelFilter(Mat src, float direction)
{
	Mat sobel(3, 3, CV_32FC1, Scalar(0));
	if (direction == 0)
	{
		sobel.at<float>(0, 0) = -1;
		sobel.at<float>(1, 0) = -1;
		sobel.at<float>(2, 0) = -1;

		sobel.at<float>(0, 2) = 1;
		sobel.at<float>(1, 2) = 1;
		sobel.at<float>(2, 2) = 1;
	}
	else if (direction == 1)
	{
		sobel.at<float>(0, 0) = -1;
		sobel.at<float>(0, 1) = -1;
		sobel.at<float>(0, 2) = -1;

		sobel.at<float>(2, 0) = 1;
		sobel.at<float>(2, 1) = 1;
		sobel.at<float>(2, 2) = 1;
	}

	Mat dst(src.size(), src.type());
	filter2D(src, dst, src.depth(), sobel);
	return dst;
}

Mat laplacianFilter(Mat src)
{
	float lap[3][3] = {
		{0, 1, 0},
		{1,-4, 1},
		{0, 1, 0}
	};
	Mat laplacian(3, 3, CV_32FC1, lap);

	Mat dst(src.size(), src.type());
	filter2D(src, dst, src.depth(), laplacian);
	return dst;
}