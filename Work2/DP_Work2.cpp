#include "DP_Work2.h"

int main(void)
{
	/*
	Mat cameraman = imread("./images/cameraman.tif", IMREAD_GRAYSCALE);
	if (cameraman.empty()) {
		cout << "Wrong dir." << endl;
		return -1;
	}
	Mat fft;
	cameraman.convertTo(cameraman, CV_32F);
	int m = getOptimalDFTSize(cameraman.rows);
	int n = getOptimalDFTSize(cameraman.cols);

	Mat padded;
	copyMakeBorder(cameraman, padded, 0, m - cameraman.rows, 0, n - cameraman.cols, BORDER_CONSTANT, Scalar::all(0));
	dft(padded, fft, DFT_COMPLEX_OUTPUT);
	fft = fftshift2d(fft);
	Mat channels[2];
	split(fft, channels);

	Mat mag;
	magnitude(channels[0], channels[1], mag);
	
	Mat normal = mag + Scalar::all(1);
	log(normal, normal);
	normalize(normal, normal, 0, 1, NORM_MINMAX);

	imshow("fft", normal);

	Mat original;
	merge(channels, 2, original);

	original = fftshift2d(original);
	dft(original, original, DFT_INVERSE);
	split(original, channels);
	normalize(channels[0], channels[0], 0, 255, NORM_MINMAX);
	channels[0].convertTo(channels[0], CV_8UC1);
	imshow("Reconstructed", channels[0]);
	waitKey(0);
	*/ 
	//example
	
	//Filtering
	/*
	Mat cameraman = imread("./images/testpattern512.tif", IMREAD_GRAYSCALE);
	if (cameraman.empty()) {
		cout << "Wrong dir." << endl;
		return -1;
	}
	imshow("original", cameraman);

	Mat cam_ideal = idealfilter(cameraman, 50);
	imshow("ideal_low", cam_ideal);
	Mat cam_idealbox = idealboxfilter(cameraman, Size(9, 9));
	imshow("ideal_low_box", cam_idealbox);
	Mat cam_gauss = gaussfilter(cameraman, 30);
	imshow("gauss_low", cam_gauss);
	Mat cam_butter = butterfilter(cameraman, 3, 30);
	imshow("butter_low", cam_butter);
	waitKey(0);

	cam_ideal = idealfilter(cameraman, 50, FILT_HIGH);
	imshow("ideal_high", cam_ideal);
	cam_gauss = gaussfilter(cameraman, 30, FILT_HIGH);
	imshow("gauss_high", cam_gauss);
	cam_butter = butterfilter(cameraman, 3, 30, FILT_HIGH);
	imshow("butter_high", cam_butter);
	waitKey(0);
	*/

	Mat wirebond = imread("./images/wirebond-mask.tif", IMREAD_GRAYSCALE);
	if (wirebond.empty()) {
		cout << "Wrong dir." << endl;
		return -1;
	}
	imshow("wirebond_original", wirebond);

	Mat structuring_element1(45, 45, CV_8UC1, Scalar(1));
	Mat wirebond_eroded = erosion(wirebond, structuring_element1, Size(22, 22));

	imshow("Eroded", wirebond_eroded);
	waitKey(0);
	
	Mat text = imread("./images/text-broken.tif", IMREAD_GRAYSCALE);
	if (wirebond.empty()) {
		cout << "Wrong dir." << endl;
		return -1;
	}
	imshow("text_original", text);

	Mat structuring_element2(3, 3, CV_8UC1, Scalar(1));
	Mat text_dilation = dilation(text, structuring_element2, Size(1, 1));
	imshow("text", text_dilation);
	waitKey(0);

	Mat thumb = imread("./images/fingerprint-noisy.tif", IMREAD_GRAYSCALE);
	if (thumb.empty()) {
		cout << "Wrong dir." << endl;
		return -1;
	}
	imshow("thumb_original", thumb);

	Mat thumb_opening = opening(thumb, structuring_element2, Size(1, 1));
	Mat thumb_closing = closing(thumb_opening, structuring_element2, Size(1, 1));
	imshow("Thumb_opening", thumb_opening);
	imshow("Thumb_opening_closing", thumb_closing);
	waitKey(0);

	Mat lincoln = imread("./images/lincoln.tif", IMREAD_GRAYSCALE);
	if (lincoln.empty()) {
		cout << "Wrong dir." << endl;
		return -1;
	}
	imshow("lincoln_original", lincoln);
	
	Mat lincoln_ero = erosion(lincoln, structuring_element2, Size(1, 1));
	Mat lincoln_bound = img_sub(lincoln_ero, lincoln);
	imshow("boundery of lincoln", lincoln_bound);
	waitKey(0);

}