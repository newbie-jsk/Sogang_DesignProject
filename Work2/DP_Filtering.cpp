#include "DP_Work2.h"

Mat filtering(Mat src, Mat kern)
{
	return src;
}

Mat get_gaussLPF(Size size, float sigma)
{
	Mat dst(size, CV_32FC2, Scalar(0));
	int mid_x = (int)floor(size.height / 2.0), mid_y = (int)floor(size.width / 2.0);

	for (int r = 0; r < size.height; r++)
	{
		for (int c = 0; c < size.width; c++)
		{
			float d_sq = (r - mid_x) * (r - mid_x) + (c - mid_y) * (c - mid_y);
			dst.at<Vec2f>(r, c)[0] = exp(-d_sq / (2 * sigma * sigma));
			dst.at<Vec2f>(r, c)[1] = dst.at<Vec2f>(r, c)[0];
		}
	}

	return dst;
}

Mat get_idealLPF(Size size, float distance)
{
	Mat dst(size, CV_32FC2, Scalar(0));
	int mid_x = (int)floor(size.height/2.0), mid_y = (int)floor(size.width/2.0);
	
	for (int r = 0; r < size.height; r++)
	{
		for (int c = 0; c < size.width; c++)
		{
			float d = sqrt((r - mid_x) * (r - mid_x) + (c - mid_y) * (c - mid_y));
			if (d < distance) {
				dst.at<Vec2f>(r, c)[0] = 1.0;
				dst.at<Vec2f>(r, c)[1] = 1.0;
			}
		}
	}

	Mat spartial = myshift(fftshift2d(ifft2d(dst)));
	normalize(spartial, spartial, 0, 1, NORM_MINMAX);
	
	return dst;
}

Mat get_idealboxLPF(Size size, Size kern_size)
{
	Mat dst(size, CV_32FC2, Scalar(0));
	Mat kern_spa(kern_size, CV_32FC1, Scalar(1));

	Mat kern;
	copyMakeBorder(kern_spa, kern, 0, size.height - kern_size.height, 0, size.width - kern_size.width, BORDER_CONSTANT, 0);
	kern = myshift(kern);
	dft(kern, dst, DFT_COMPLEX_OUTPUT);

	Mat chan[2];
	split(dst, chan);

	Mat mag;
	magnitude(chan[0], chan[1], mag);

	Mat mags[2] = { mag, mag };
	merge(mags, 2, mag);

	return mag;
}

Mat get_butterworthLPF(Size size, int order, float distance)
{
	Mat dst(size, CV_32FC2, Scalar(0));
	int mid_x = (int)floor(size.height / 2.0), mid_y = (int)floor(size.width / 2.0);

	for (int r = 0; r < size.height; r++)
	{
		for (int c = 0; c < size.width; c++)
		{
			float d = sqrt((r - mid_x) * (r - mid_x) + (c - mid_y) * (c - mid_y));
			dst.at<Vec2f>(r, c)[0] = (float)(1.0 / (1.0 + pow((d / distance), 2 * order)));
			dst.at<Vec2f>(r, c)[1] = dst.at<Vec2f>(r, c)[0];
		}
	}

	return dst;
}

/*
	1. Zero Padding
	2. Shift to center (Mult (-1)^(r+c))
	3. DFT
	4. Get kernel
	5. Image_fft * kern
	6. IDFT & re shift
	7. Crop Image & return to uchar
	*/
Mat idealfilter(Mat src, int distance, FILT_TYPE type)
{
	int r = getOptimalDFTSize(src.rows);
	int c = getOptimalDFTSize(src.cols);
	Mat dst;
	copyMakeBorder(src, dst, 0, r - src.rows, 0, c - src.cols, BORDER_CONSTANT, 0);
	dst.convertTo(dst, CV_32F);
	dst = myshift(dst);
	dft(dst, dst, DFT_COMPLEX_OUTPUT);

	Mat kern = get_idealLPF(dst.size(), distance);
	if (type == FILT_HIGH) {
		Mat mat_ones(kern.size(), kern.type(), Scalar(1, 1));
		kern = mat_ones - kern;
	}
	dst = dst.mul(kern);

	//fft_show(kern);
	//fft_show(dst);

	dst = myshift(ifft2d(dst));
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	dst.convertTo(dst, CV_8U);

	Rect mask(0, 0, src.cols, src.rows); // Resize
	dst = dst(mask);
	return dst;
}

Mat idealboxfilter(Mat src, Size size, FILT_TYPE type)
{
	int r = getOptimalDFTSize(src.rows);
	int c = getOptimalDFTSize(src.cols);
	Mat dst;
	copyMakeBorder(src, dst, 0, r - src.rows, 0, c - src.cols, BORDER_CONSTANT, 0);
	dst.convertTo(dst, CV_32F);
	dst = myshift(dst);
	dft(dst, dst, DFT_COMPLEX_OUTPUT);

	Mat kern = get_idealboxLPF(dst.size(), size);
	if (type == FILT_HIGH) {
		Mat mat_ones(kern.size(), kern.type(), Scalar(1, 1));
		kern = mat_ones - kern;
	}
	dst = dst.mul(kern);

	//fft_show(kern);
	//fft_show(dst);

	dst = myshift(ifft2d(dst));
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	dst.convertTo(dst, CV_8U);

	Rect mask(0, 0, src.cols, src.rows); // Resize
	dst = dst(mask);
	return dst;
}

/*
	1. Zero Padding
	2. Shift to center (Mult (-1)^(r+c))
	3. DFT
	4. Get kernel
	5. Image_fft * kern
	6. IDFT & re shift
	7. Crop Image & return to uchar
	*/
Mat gaussfilter(Mat src, float sigma, FILT_TYPE type)
{
	int r = getOptimalDFTSize(src.rows);
	int c = getOptimalDFTSize(src.cols);
	Mat dst;
	copyMakeBorder(src, dst, 0, r - src.rows, 0, c - src.cols, BORDER_CONSTANT, 0);
	dst.convertTo(dst, CV_32F);
	dst = myshift(dst);
	dft(dst, dst, DFT_COMPLEX_OUTPUT);

	Mat kern = get_gaussLPF(dst.size(), sigma);
	if (type == FILT_HIGH) {
		Mat mat_ones(kern.size(), kern.type(), Scalar(1, 1));
		kern = mat_ones - kern;
	}
	dst = dst.mul(kern);

	//fft_show(kern);
	//fft_show(dst);

	dst = myshift(ifft2d(dst));
	normalize(dst, dst, 0, 255, NORM_MINMAX);
	dst.convertTo(dst, CV_8U);

	Rect mask(0, 0, src.cols, src.rows); // Resize
	dst = dst(mask);
	return dst;
}

/*
	1. Zero Padding
	2. Shift to center (Mult (-1)^(r+c))
	3. DFT
	4. Get kernel
	5. Image_fft * kern
	6. IDFT & re shift
	7. Crop Image & return to uchar
	*/
Mat butterfilter(Mat src, int order, float distance, FILT_TYPE type)
{
	int r = getOptimalDFTSize(src.rows);
	int c = getOptimalDFTSize(src.cols);
	Mat dst;
	copyMakeBorder(src, dst, 0, r - src.rows, 0, c - src.cols, BORDER_CONSTANT, 0); //Zero padding
	dst.convertTo(dst, CV_32F);
	dst = myshift(dst);					// shift to center
	dft(dst, dst, DFT_COMPLEX_OUTPUT);	//dft

	Mat kern = get_butterworthLPF(dst.size(), order, distance); // Generate kernel
	if (type == FILT_HIGH) {
		Mat mat_ones(kern.size(), kern.type(), Scalar(1, 1));
		kern = mat_ones - kern;
	}
	dst = dst.mul(kern); //Multiply

	//fft_show(dst);

	dst = myshift(ifft2d(dst)); // Inverse DFT & decentering
	normalize(dst, dst, 0, 255, NORM_MINMAX); // Normalize float to uchar
	dst.convertTo(dst, CV_8U);

	Rect mask(0, 0, src.cols, src.rows); // Resize
	dst = dst(mask);
	return dst;
}

void fft_show(Mat src)
{
	int type = src.type();
	if (type == CV_32FC2)
	{
		Mat chan[2];
		split(src, chan);
		Mat mag, pha;
		magnitude(chan[0], chan[1], mag);
		phase(chan[0], chan[1], pha);

		mag += Scalar::all(1);
		pha += Scalar::all(1);
		log(mag, mag);
		log(pha, pha);

		normalize(mag, mag, 0, 1, NORM_MINMAX);
		normalize(pha, pha, 0, 1, NORM_MINMAX);

		imshow("magnitude", mag);
		imshow("phase", pha);
		waitKey(0);
	}
	else
	{
		cout << "wrong type" << endl;
	}
}

Mat myshift(Mat src)
{
	Mat dst(src.size(), src.type());
	for (int r = 0; r < src.rows; r++)
	{
		for (int c = 0; c < src.cols; c++)
		{
			if ((r + c) % 2)
				dst.at<float>(r, c) = src.at<float>(r, c);
			else
				dst.at<float>(r, c) = src.at<float>(r, c) * (-1);
		}
	}

	return dst;
}

Mat complex_mult(Mat src1_fft, Mat src2_fft)
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