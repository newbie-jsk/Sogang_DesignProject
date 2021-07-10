#include "DP_Work1.h"

Mat resize(Mat src, int x, int y)
{
	Mat dst(src.rows + x, src.cols + y, src.type());

	return dst;
}

//float * float instruction
Mat mat_mult(Mat kern, Mat src)
{
	if (kern.cols != src.rows) {
		cout << "Matrix size error\n" << endl;
		return src;
	}

	Mat kern_f = kern.clone();
	Mat src_f = src.clone();
	kern_f.convertTo(kern_f, CV_32FC1);
	src_f.convertTo(src_f, CV_32FC1);

	Mat dst(kern.rows, src.cols, CV_32FC1, Scalar(0));

	for (int r = 0; r < dst.rows; r++)
	{
		for (int c = 0; c < dst.cols; c++)
		{
			for (int i = 0; i < kern.cols; i++)
			{
				dst.at<float>(r, c) += (kern_f.at<float>(r, i) * src_f.at<float>(i, c));
			}
		}
	}

	return dst;
}

Mat mat_mult_comp(Mat src1_fft, Mat src2_fft)
{
	Mat dst(src1_fft.size(), CV_32FC2);

	// Real part calc
	for (int i = 0; i < src1_fft.rows; i++)
	{
		for (int j = 0; j < src1_fft.cols; j++)
		{
			dst.at<Vec2f>(i, j)[0] = src1_fft.at<Vec2f>(i, j)[0] * src2_fft.at<Vec2f>(i, j)[0];
			dst.at<Vec2f>(i, j)[0] += src1_fft.at<Vec2f>(i, j)[1] * src2_fft.at<Vec2f>(i, j)[1];
		}
	}

	//Img part calc
	for (int i = 0; i < src1_fft.rows; i++)
	{
		for (int j = 0; j < src1_fft.cols; j++)
		{
			dst.at<Vec2f>(i, j)[1] = src1_fft.at<Vec2f>(i, j)[0] * src2_fft.at<Vec2f>(i, j)[1];
			dst.at<Vec2f>(i, j)[1] += src1_fft.at<Vec2f>(i, j)[1] * src2_fft.at<Vec2f>(i, j)[0];
		}
	}

	return dst;
}


Mat scale(Mat src, float scale, interpolation interp)
{
	float data[3][3] = {
		{scale, 0, 0},
		{0, scale, 0},
		{0, 0, 1}
	};

	Mat kernel(3, 3, CV_32FC1, data);
	return aff_transform(src, kernel, interp);
}

Mat rotate(Mat src, float rad, interpolation interp)
{
	float data[3][3] = {
		{cos(rad), sin(rad), 0},
		{-sin(rad), cos(rad), 0},
		{0, 0, 1}
	};

	Mat kernel(3, 3, CV_32FC1, data);

	return aff_transform(src, kernel, interp);
}

Mat translate(Mat src, int x, int y, interpolation interp)
{
	float data[3][3] = {
		{1, 0, x},
		{0, 1, y},
		{0, 0, 1}
	};

	Mat kernel(3, 3, CV_32FC1, data);

	return aff_transform(src, kernel, interp);
}

