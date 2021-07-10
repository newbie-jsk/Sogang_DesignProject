#include "DP_Work1.h"
#define PI 3.14159265

int main()
{
	/*
	Mat letterB = imread("./images/letterB.tif", 0);
	if (letterB.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}

	imshow("origin", letterB);

	Mat B_shift = translate(letterB, 50, 50, BICUBIC);
	imshow("Translated", B_shift);
	imwrite("Translated.tif", B_shift);
	Mat B_scale = scale(letterB, 1.7, BICUBIC);
	imshow("scaled", B_scale);
	imwrite("scaled.tif", B_scale);
	Mat B_nearest = rotate(letterB, PI/6.0, NEAREST);
	imshow("near", B_nearest);
	imwrite("near.tif", B_nearest);
	Mat B_bilinear = rotate(letterB, PI/6.0, BILINEAR);
	imshow("bilinear", B_bilinear);
	imwrite("bilinear.tif", B_bilinear);
	Mat B_bicubic = rotate(letterB, PI/6.0, BICUBIC);
	imshow("bicubic", B_bicubic);
	imwrite("bicubic.tif", B_bicubic);

	B_bilinear = scale(B_bilinear, 0.1, BILINEAR);
	B_bicubic = scale(B_bicubic, 0.1, BILINEAR);

	//cout << B_bilinear << endl;
	//cout << B_bicubic << endl;
	//waitKey(0);
	*/
	Mat finger = imread("./images/fingerprint.tif", 0);
	if (finger.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}

	Mat finger_threshold = threshold_my(finger, BASIC);

	imshow("finger_original", finger);
	imshow("finger_thresholded", finger_threshold);
	imwrite("finger_original.tif", finger);
	imwrite("finger_thresholded.tif", finger_threshold);
	waitKey(0);
	

	Mat cell = imread("./images/polymercell.tif", 0);
	if (cell.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}
	imshow("cell_origin", cell);
	imwrite("cell_origin.tif", cell);
	Mat cell_basic = threshold_my(cell, BASIC);
	imshow("cell_basic", cell_basic);
	imwrite("cell_basic.tif", cell_basic);
	Mat cell_otsu = threshold_my(cell, OTSU);
	imshow("cell_otsu", cell_otsu);
	imwrite("cell_otsu.tif", cell_otsu);
	waitKey(0);

	
	Mat cepta = imread("./images/septa-noise50.tif", 0);
	if (cepta.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}
	imshow("cepta_origin", cepta);
	imwrite("cepta_origin.tif", cepta);
	Mat cepta_otsu = threshold_my(cepta, OTSU);
	imshow("cepta_otsu", cepta_otsu);
	imwrite("cepta_otsu.tif", cepta_otsu);

	Mat cepta_filt;
	boxFilter(cepta, cepta_filt, -1, Size(5, 5));
	imshow("filtered", cepta_filt);
	imwrite("filtered.tif", cepta_filt);
	Mat cepta_smooth = threshold_my(cepta_filt, OTSU);
	imshow("cepta_smooth", cepta_smooth);
	imwrite("cepta_smooth.tif", cepta_smooth);
	waitKey(0);
	

	Mat cepta_small = imread("./images/septagon-small.tif", 0);
	if (cepta_small.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}

	Mat cepta_small_t = edge_threshold(cepta_small, 0.997, 0);

	imshow("cepta_small", cepta_small);
	imshow("cepta_small_t", cepta_small_t);
	waitKey(0);

	Mat yeast = imread("./images/yeast-cells.tif", 0);
	if (yeast.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}
	Mat yeast_otsu = threshold_my(yeast, OTSU);
	imshow("yeast_otsu", yeast_otsu);
	Mat yeast_lap = edge_threshold(yeast, 0.995, 1);
	imshow("yeast_lap", yeast_lap);
	imwrite("yeast_otsu.tif", yeast_otsu);
	imwrite("yeast_lap.tif", yeast_lap);
	waitKey(0);

	Mat iceberg = imread("./images/iceberg.tif", 0);
	if (iceberg.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}

	Mat iceberg_mult = multi_threshold(iceberg);
	imshow("iceberg_origin", iceberg);
	imshow("iceberg_thresholded", iceberg_mult);
	imwrite("iceberg_origin.tif", iceberg);
	imwrite("iceberg_thresholded.tif", iceberg_mult);
	float hist[256] = { 0 };
	getHist(iceberg, hist);
	histPrint(hist);
	waitKey(0);

	Mat spot = imread("./images/text-spotshade.tif", 0);
	if (spot.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}
	Mat spot_th = ma_threshold(spot, 20, 0.5);
	Mat spot_otsu = threshold_my(spot, OTSU);
	imshow("text_spot", spot);
	imshow("text_otsu", spot_otsu);
	imshow("text_thres", spot_th);
	imwrite("text_spot.tif", spot);
	imwrite("text_otsu.tif", spot_otsu);
	imwrite("text_thres.tif", spot_th);
	waitKey(0);


	Mat sine = imread("./images/text-sineshade.tif", 0);
	if (sine.empty())
	{
		cout << "Wrong file name or directory" << endl;
		exit(-1);
	}
	Mat sine_th = ma_threshold(sine, 20, 0.5);
	Mat sine_otsu = threshold_my(sine, OTSU);
	imshow("text_sineshade", sine);
	imshow("text_sine_otsu", sine_otsu);
	imshow("text_sine_ma", sine_th);
	imwrite("text_sineshade.tif", sine);
	imwrite("text_sine_otsu.tif", sine_otsu);
	imwrite("text_sine_ma.tif", sine_th);
	waitKey(0);
}
