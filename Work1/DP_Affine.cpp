#include "DP_Work1.h"


Mat aff_transform(Mat src, Mat aff_mat, interpolation interp)
{
	// Step 1 : Resize Image.
	// 1. Calculate transformed corners 
	// 2. Using that data, resize image.

	Mat aff_mat_f;
	aff_mat.convertTo(aff_mat_f, CV_32FC1);

	int data[3][4] = {
		{0, src.rows,	0		,src.rows },
		{0,		0	, src.cols	,src.cols },
		{1,		1	,	1		,	1	  }
	};
	Mat src_corner(3, 4, CV_32SC1, data);
	src_corner = mat_mult(aff_mat_f, src_corner);

	float x_min = 0, x_max = 0 , y_min = 0, y_max = 0;

	for (int c = 0; c < 4; c++)
	{
		if (src_corner.at<float>(0, c) < x_min)
			x_min = src_corner.at<float>(0, c);
		else if (src_corner.at<float>(0, c) > x_max)
			x_max = src_corner.at<float>(0, c);

		if (src_corner.at<float>(1, c) < y_min)
			y_min = src_corner.at<float>(1, c);
		else if (src_corner.at<float>(1, c) > y_max)
			y_max = src_corner.at<float>(1, c);
	}
	x_max = x_max - src.rows;
	y_max = y_max - src.cols;

	Mat dst = resize(src, (int)ceil(x_max - x_min), (int)ceil(y_max - y_min));

	// Step 2 : Transform image by Inverse matrix.

	aff_mat_f.at<float>(0, 2) += ceil(abs(x_min));
	aff_mat_f.at<float>(1, 2) += ceil(abs(y_min));

	Mat aff_mat_inv = aff_mat_f.inv();

	for (int r = 0; r < dst.rows; r++)
	{
		for (int c = 0; c < dst.cols; c++)
		{
			float origin_x, origin_y;
			origin_x = aff_mat_inv.at<float>(0, 0) * r + aff_mat_inv.at<float>(0, 1) * c + aff_mat_inv.at<float>(0, 2);
			origin_y = aff_mat_inv.at<float>(1, 0) * r + aff_mat_inv.at<float>(1, 1) * c + aff_mat_inv.at<float>(1, 2);

			if (interp == NEAREST) {
				dst.at<uchar>(r, c) = (uchar)round(nearest(src, origin_x, origin_y));
			}
			else if (interp == BILINEAR) {
				dst.at<uchar>(r, c) = (uchar)round(bilinear(src, origin_x, origin_y));
			}
			else if (interp == BICUBIC) {
				dst.at<uchar>(r, c) = (uchar)round(bicubic(src, origin_x, origin_y));
			}

		}
	}



	return dst;
}

float nearest(Mat src, float x, float y)
{
	if (x < 0 || x >(src.rows - 1) || y < 0 || y >(src.cols - 1)) {
		return 0;
	}

	int x_int = (int)round(x);
	int y_int = (int)round(y);

	return src.at<uchar>(x_int, y_int);
}

float bilinear(Mat src, float x, float y)
{
	if (x < 0 || x >(src.rows - 1) || y < 0 || y >(src.cols - 1)) {
		return 0;
	}
	int loc_x0, loc_x1, loc_y0, loc_y1;

	loc_x0 = (int)floor(x);
	loc_x1 = (int)ceil(x);
	loc_y0 = (int)floor(y);
	loc_y1 = (int)ceil(y);

	float A, B, C, D;
	
	A = src.at<uchar>(loc_x0, loc_y0);
	B = src.at<uchar>(loc_x0, loc_y1);
	C = src.at<uchar>(loc_x1, loc_y0);
	D = src.at<uchar>(loc_x1, loc_y1);

	float dx = x - loc_x0;
	float dy = y - loc_y0;

	return (A * (1 - dx) * (1 - dy) + B * (1 - dx) * dy + C * (1 - dy) * dx + D * dx * dy);
}

float bicubic(Mat src, float x, float y)
{
	if (x < 0 || y < 0 || x >=(src.rows - 1) || y >=(src.cols - 1)) {
		return 0;
	}

	int x_floor, y_floor;
	x_floor = (int)floor(x), y_floor = (int)floor(y);
	float dx = x - x_floor, dy = y - y_floor;

	float p[4][4] = { 0 };

	int r, c, r_start, r_end, c_start, c_end;
	if (x_floor == 0)
		r_start = 0, r_end = 3;
	else if (x_floor == src.rows - 2)
		r_start = -1, r_end = 2;
	else
		r_start = -1, r_end = 3;
		
	if (y_floor == 0)
		c_start = 0, c_end = 3;
	else if (y_floor == src.cols - 2)
		c_start = -1, c_end = 2;
	else
		c_start = -1, c_end = 3;
	
	for (r = r_start; r < r_end; r++)
	{
		for (c = c_start; c < c_end; c++)
		{
			p[r + 1][c + 1] = (float)src.at<uchar>(x_floor + r, y_floor + c);
		}
	}

	float result = 
		cubic(
			cubic(p[0][0], p[0][1], p[0][2], p[0][3], dy),
			cubic(p[1][0], p[1][1], p[1][2], p[1][3], dy),
			cubic(p[2][0], p[2][1], p[2][2], p[2][3], dy),
			cubic(p[3][0], p[3][1], p[3][2], p[3][3], dy),
			dx
			);
	if (result > 255) { result = 255; }
	else if (result < 0) { result = 0; }
	return result;
}

float cubic(float y0, float y1, float y2, float y3, float dx)
{
	float a, b, c, d;
	a = -y0 / 6.0 + y1 / 2.0 - y2 / 2.0 + y3 / 6.0;
	b = y0 / 2.0 - y1 + y2 / 2.0;
	c = -y0 / 3.0 - y1 / 2.0 + y2 - y3 / 6.0;
	d = y1;

	return (d + (c + (b + a * dx) * dx) * dx);
}