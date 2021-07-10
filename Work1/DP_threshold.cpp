#include "DP_Work1.h"

Mat threshold_my(Mat src, t_thres type)
{
	float hist[256] = { 0 };
	uchar T;

	switch (type) {
	case BASIC:
		getHist(src, hist);
		T = basic_threshold(hist);
		break;
	case OTSU:
		getHist(src, hist);
		T = otsu_threshold(hist);
		break;
	}

	Mat dst(src.size(), src.type());
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (src.at<uchar>(r, c) < T)
				dst.at<uchar>(r, c) = 0;
			else if (src.at<uchar>(r, c) >= T)
				dst.at<uchar>(r, c) = 255;
		}
	}

	return dst;
}

uchar basic_threshold(float* hist)
{
	uchar T;
	uchar dT = 3;
	uchar T_prev;

	float mean1 = 0, mean2 = 0, prob = 0;
	for (int i = 0; i < 256; i++)
	{
		mean1 += hist[i] * i;
	}
	T = (uchar)mean1;

	while (1)
	{
		mean1 = 0, prob = 0;
		for (int i = 0; i <= T; i++)
		{
			mean1 += hist[i] * i;
			prob += hist[i];
		}
		mean1 = mean1 / prob;

		mean2 = 0, prob = 0;
		for (int i = T+1; i < 256; i++)
		{
			mean2 += hist[i] * i;
			prob += hist[i];
		}
		mean2 = mean2 / prob;

		T_prev = T;
		T = (mean1 + mean2) / 2.0;
		
		if ((uchar)abs(T_prev - T) < dT)
			break;
	}

	return T;
}

uchar otsu_threshold(float* hist)
{
	uchar max_k = 0;
	float mean = 0, prob1 = 0, glob_mean = 0;
	float var_b, max_var_b = 0;
	int num = 1;

	for (int i = 1; i < 256; i++) {
		glob_mean += hist[i] * i;
	}

	// find max var_b
	for (int k = 0; k < 256; k++)
	{
		mean = 0, prob1 = 0;
		for (int j = 0; j <= k; j++) {
			mean += hist[j] * j;
			prob1 += hist[j];
		}
		if (prob1 == 0 || prob1 == 1)
			var_b = 0;
		else
			var_b = (glob_mean * prob1 - mean) * (glob_mean * prob1 - mean) / (prob1 * (1 - prob1));

		if (var_b > max_var_b) {
			max_k = k;
			max_var_b = var_b;
			num = 1;
		}
		else if (var_b == max_var_b)
		{
			max_k = (max_k * num + k) / (num + 1);
			num += 1;
		}
	}
	
	return max_k;
}

Mat multi_threshold(Mat src)
{
	float hist[256] = { 0 };
	getHist(src, hist);

	float max_var_b = 0, var_b;
	uchar max_k1 = 0, max_k2 = 1;

	for (int i = 1; i < 255; i++)
	{
		for (int j = i + 1; j < 256; j++)
		{
			var_b = sig_b(hist, i, j);
			if (max_var_b < var_b) {
				max_var_b = var_b;
				max_k1 = (uchar)i;
				max_k2 = (uchar)j;
			}
		}
	}

	Mat dst(src.size(), src.type());
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (src.at<uchar>(r, c) < max_k1)
				dst.at<uchar>(r, c) = 0;
			else if (src.at<uchar>(r, c) >= max_k2)
				dst.at<uchar>(r, c) = 255;
			else
				dst.at<uchar>(r, c) = 128;
		}
	}

	return dst;
}

Mat edge_threshold(Mat src, float percent, int mode)
{
	Mat grad;
	if (mode == 0) {
		Sobel(src, grad, CV_32F, 1, 1, 3);
		//for (int r = 0; r < src.rows; r++)
		//	for (int c = 0; c < src.cols; c++)
		//		grad.at<float>(r, c) = abs(grad.at<float>(r, c));
	}
	else
		Laplacian(src, grad, CV_8U, 3);

	float hist[256] = { 0 };
	grad.convertTo(grad, CV_8U);
	Mat mask = percent_threshold(grad, percent);
	Mat masked_src = src.mul(mask);
	
	getHist(masked_src, hist);
	hist[0] = 0;

	uchar th = otsu_threshold(hist);

	histPrint(hist);

	Mat dst(src.size(), src.type());
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (src.at<uchar>(r, c) < th)
				dst.at<uchar>(r, c) = 0;
			else
				dst.at<uchar>(r, c) = 255;
		}
	}

	return dst;
}

Mat ma_threshold(Mat src, int n, float num)
{
	float mean = 0;
	uchar* data = src.data;
	Mat th(src.size(), CV_8UC1, Scalar(0));
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			mean += (float)data[r * src.cols + c] / (float)n;
			if (r * src.cols + c >= n) {
				mean -= (float)data[r * src.cols + c - n] / (float)n;
			}
			th.at<uchar>(r, c) = (uchar)(num * mean);
		}
	}

	Mat dst(src.size(), src.type());
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (th.at<uchar>(r, c) < src.at<uchar>(r, c))
				dst.at<uchar>(r, c) = 255;
			else
				dst.at<uchar>(r, c) = 0;
		}
	}
	return dst;
}

float sig_b(float* hist, uchar k1, uchar k2)
{
	float prob1, prob2, prob3;
	float mean1, mean2, mean3;

	mean1 = 0, prob1 = 0;
	for (int i = 0; i < k1; i++)
	{
		mean1 += hist[i] * i;
		prob1 += hist[i];
	}
	mean1 = mean1 / prob1;
	
	mean2 = 0, prob2 = 0;
	for (int i = k1; i < k2; i++)
	{
		mean2 += hist[i] * i;
		prob2 += hist[i];
	}
	mean2 = mean2 / prob2;

	mean3 = 0, prob3 = 0;
	for (int i = k2; i < 256; i++)
	{
		mean3 += hist[i] * i;
		prob3 += hist[i];
	}
	mean3 = mean3 / prob3;

	float mean_g = mean1 * prob1 + mean2 * prob2 + mean3 * prob3;
	
	return (prob1 * (mean1 - mean_g) * (mean1 - mean_g) + prob2 * (mean2 - mean_g) * (mean2 - mean_g) + prob3 * (mean3 - mean_g) * (mean3 - mean_g));
}

Mat percent_threshold(Mat src, float percentile)
{
	float hist[256] = { 0 };
	getHist(src, hist);

	float sum = 0;
	int i = 0;
	while (percentile > sum)
		sum += hist[i++];
	
	Mat dst(src.size(), CV_8UC1);
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if (src.at<uchar>(r, c) > i)
				dst.at<uchar>(r, c) = 1;
			else
				dst.at<uchar>(r, c) = 0;
		}
	}

	return dst;
}

void getHist(Mat src, float hist[])
{
	///Variables Declaration///
	uchar* data;
	uchar intensity_level;
	double one_div_MN;
	double hist_d[256] = { 0 };
	data = src.data;							//Img data type
	one_div_MN = 1.0 / ((float)src.cols * (float)src.rows);	// 1/Img size

	///Cummulate intensity data by inspecting all pixels 
	/// -> histogram
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			intensity_level = data[i * src.cols + j];
			hist_d[intensity_level] += one_div_MN;
		}
	}

	for (int i = 0; i < 256; i++)
		hist[i] = (float)hist_d[i];
}

void histPrint(float* hist)
{
	cout << '[';
	for (int i = 0; i < 255; i++) {
		cout << hist[i] << ", ";
	}
	cout << hist[255] << ']' << endl;
}