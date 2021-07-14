#include "DP_Work2.h"

Mat erosion(Mat src, Mat se, Size se_center)
{
	Mat dst(src.size(), src.type(), Scalar(0));
	Mat src_padded;
	int x_inc = (se.rows - 1) / 2, y_inc = (se.cols - 1) / 2;
	copyMakeBorder(src, src_padded, y_inc, y_inc, x_inc, x_inc, BORDER_REPLICATE);

	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			int istrue = 1;
			for (int r_se = 0; r_se < se.rows; r_se++)
			{
				for (int c_se = 0; c_se < se.cols; c_se++)
				{
					if (se.at<uchar>(r_se, c_se) == 1 && src_padded.at<uchar>(r + r_se, c + c_se) != 255)
					{
						istrue = 0;
						goto erosion_break;
					}
					else if (se.at<uchar>(r_se, c_se) == 0 && src_padded.at<uchar>(r + r_se, c + c_se) != 0)
					{
						istrue = 0;
						goto erosion_break;
					}
				}
			}

			erosion_break:
			if (istrue)
				dst.at<uchar>(r + se_center.height - x_inc, c + se_center.width - y_inc) = 255;
		}
	}

	return dst;
}

Mat dilation(Mat src, Mat se, Size se_center)
{
	Mat dst(src.size(), src.type(), Scalar(0));
	Mat src_padded;
	int x_inc = (se.rows - 1) / 2, y_inc = (se.cols - 1) / 2;
	copyMakeBorder(src, src_padded, y_inc, y_inc, x_inc, x_inc, BORDER_REPLICATE);

	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			int istrue = 0;
			for (int r_se = 0; r_se < se.rows; r_se++)
			{
				for (int c_se = 0; c_se < se.cols; c_se++)
				{
					if (se.at<uchar>(r_se, c_se) == 1 && src_padded.at<uchar>(r + r_se, c + c_se) == 255)
					{
						istrue = 1;
						goto dilation_break;
					}
					else if (se.at<uchar>(r_se, c_se) == 0 && src_padded.at<uchar>(r + r_se, c + c_se) == 0)
					{
						istrue = 1;
						goto dilation_break;
					}
				}
			}
			
			dilation_break:
			if (istrue)
				dst.at<uchar>(r + se_center.height - x_inc, c + se_center.width - y_inc) = 255;
		}
	}

	return dst;
}

Mat opening(Mat src, Mat se, Size se_center)
{
	return dilation(erosion(src, se, se_center), se, se_center);
}

Mat closing(Mat src, Mat se, Size se_center)
{
	return erosion(dilation(src, se, se_center), se, se_center);
}

Mat img_complement(Mat src)
{
	Mat dst(src.size(), src.type());
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			dst.at<uchar>(r, c) = 255 - src.at<uchar>(r, c);
		}
	}

	return dst;
}

Mat img_sub(Mat src1, Mat src2)
{
	if (src1.size() == src2.size())
	{
		Mat dst(src1.size(), src1.type());
		for (int r = 0; r < dst.rows; r++)
		{
			for (int c = 0; c < dst.cols; c++)
			{
				dst.at<uchar>(r, c) = src1.at<uchar>(r, c) ^ src2.at<uchar>(r, c);
			}
		}
		return dst;
	}
	else
	{
		cout << "Size missmatch" << endl;
		return src1;
	}
}

Mat img_add(Mat src1, Mat src2)
{
	if (src1.size() == src2.size())
	{
		Mat dst(src1.size(), src1.type());
		for (int r = 0; r < dst.rows; r++)
		{
			for (int c = 0; c < dst.cols; c++)
			{
				dst.at<uchar>(r, c) = src1.at<uchar>(r, c) | src2.at<uchar>(r, c);
			}
		}
		return dst;
	}
	else
	{
		cout << "Size missmatch" << endl;
		return src1;
	}
}