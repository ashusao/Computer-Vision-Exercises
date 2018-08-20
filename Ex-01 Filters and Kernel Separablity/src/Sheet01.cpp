/*
 * Sheet01.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: ashu
 */

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

//	==================== Load image =========================================
	// Read the image file - bonn.png is of type CV_8UC3
	const Mat bonn = imread(argv[1], IMREAD_COLOR);

	// Create gray version of bonn.png
	Mat bonn_gray;
	cvtColor(bonn, bonn_gray, CV_BGR2GRAY);
//	=========================================================================
//	==================== Solution of task 1 =================================
//	=========================================================================
	cout << "Task 1:" << endl;
	//====(a)==== draw random rectangles ====

	// Create a copy of the input image to draw the rectangles into
	Mat img_task1;
	bonn_gray.copyTo(img_task1);

	// Create a random number generator
	RNG rng(getTickCount());
	Rect rects[100];
	for (auto& r : rects) {
		// Random coordinates of the rectangle
		const int x1 = rng.uniform(0, bonn_gray.cols);
		const int x2 = rng.uniform(0, bonn_gray.cols);
		const int y1 = rng.uniform(0, bonn_gray.rows);
		const int y2 = rng.uniform(0, bonn_gray.rows);

		//TODO: Draw the rectangle into the image "img_task1"
		// ...
		r = Rect(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));

		rectangle(img_task1, r, Scalar(255));
	}
	//TODO: Display the image "img_task1" with the rectangles
	// ...
	namedWindow("Task_1", WINDOW_AUTOSIZE);
	imshow("Task 1", img_task1);

	//====(b)==== summing up method ====
	// Variables for time measurements
	int64_t tick, tock;
	double sumOfPixels;

	// Measure the time
	tick = getTickCount();

	// Repeat 10 times
	for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
		for (auto& r : rects) {
			for (int y = r.y; y < r.y + r.height; ++y) {
				for (int x = r.x; x < r.x + r.width; ++x) {
					//TODO: Sum up all pixels at "bonn_gray.at<uchar>(y,x)" inside the rectangle and store it in "sumOfPixels"
					// ...
					sumOfPixels += bonn_gray.at<uchar>(y, x);
				}
			}
		}
	}
	tock = getTickCount();
	cout << "Summing up each pixel gives " << sumOfPixels << " computed in "
			<< (tock - tick) / getTickFrequency() << " seconds." << endl;

	// //====(c)==== integral image method - using OpenCV function "integral" ====

	//TODO: implement your solution of here
	// ...

	tick = getTickCount();

	Mat integralImage;
	integral(bonn_gray, integralImage, CV_64F);

	for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
		for (auto& r : rects) {
			sumOfPixels += integralImage.at<double>(r.y, r.x)
					+ integralImage.at<double>(r.y + r.height, r.x + r.width)
					- integralImage.at<double>(r.y + r.height, r.x)
					- integralImage.at<double>(r.y, r.x + r.width);
		}
	}
	tock = getTickCount();
	cout << "Summing up using opencv integral() method " << sumOfPixels
			<< " computed in " << (tock - tick) / getTickFrequency()
			<< " seconds." << endl;

	//====(d)==== integral image method - custom implementation====
	//TODO: implement your solution here
	// ...

	tick = getTickCount();

	Mat customIntegralImage(bonn_gray.rows + 1, bonn_gray.cols + 1, CV_64F,
			Scalar(0.0));

	for (int y = 1; y < customIntegralImage.rows; ++y) {
		for (int x = 1; x < customIntegralImage.cols; ++x) {
			customIntegralImage.at<double>(y, x) =
					customIntegralImage.at<double>(y - 1, x)
							+ customIntegralImage.at<double>(y, x - 1)
							- customIntegralImage.at<double>(y - 1, x - 1)
							+ bonn_gray.at<uchar>(y, x);
		}
	}

	for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
		for (auto& r : rects) {
			sumOfPixels += customIntegralImage.at<double>(r.y, r.x)
					+ customIntegralImage.at<double>(r.y + r.height,
							r.x + r.width)
					- customIntegralImage.at<double>(r.y + r.height, r.x)
					- customIntegralImage.at<double>(r.y, r.x + r.width);
		}
	}
	tock = getTickCount();
	cout << "Summing up using custom method " << sumOfPixels << " computed in "
			<< (tock - tick) / getTickFrequency() << " seconds." << endl;

	waitKey(0); // waits until the user presses a button and then continues with task 2 -> uncomment this
	destroyAllWindows(); // closes all open windows -> uncomment this

//	=========================================================================
//	==================== Solution of task 2 =================================
//	=========================================================================
	cout << "Task 2:" << endl;

	//====(a)==== Histogram equalization - using opencv "equalizeHist" ====
	Mat ocvHistEqualization;
	//TODO: implement your solution of here
	// ...
	equalizeHist(bonn_gray, ocvHistEqualization);

	//====(b)==== Histogram equalization - custom implementation ====
	Mat myHistEqualization(bonn_gray.size(), bonn_gray.type());

	// Count the frequency of intensities
	Vec<double, 256> hist(0.0);
	for (int y = 0; y < bonn_gray.rows; ++y) {
		for (int x = 0; x < bonn_gray.cols; ++x) {
			++hist[bonn_gray.at<uchar>(y, x)];
		}
	}
	// Normalize vector of new intensity values to sum up to a sum of 255
	hist *= 255. / sum(hist)[0];

	// Compute integral histogram - representing the new intensity values
	for (size_t i = 1; i < 256; ++i) {
		hist[i] += hist[i - 1];
	}

	// TODO: Fill the matrix "myHistEqualization" -> replace old intensity values with new intensities taken from the integral histogram
	// ...

	Mat lookupTable(hist);
	lookupTable.convertTo(lookupTable, bonn_gray.type());

	LUT(bonn_gray, lookupTable, myHistEqualization);

	// TODO: Show the results of (a) and (b)
	// ...

	imshow("Opencv Hist Equalization", ocvHistEqualization);
	imshow("My Hist Equalization", myHistEqualization);


//	imwrite("images/ocv_Hist.png",ocvHistEqualization);
//	imwrite("images/custom_Hist.png",myHistEqualization);

	//====(c)==== Difference between openCV implementation and custom implementation ====
	// TODO: Compute absolute differences between pixel intensities of (a) and (b) using "absdiff"
	// ... Just uncomment the following lines:
	Mat diff;
	absdiff(myHistEqualization, ocvHistEqualization, diff);
	double minVal, maxVal;
	minMaxLoc(diff, &minVal, &maxVal);
	cout << "maximum pixel error: " << maxVal << endl;

	waitKey(0);
	destroyAllWindows();

//	=========================================================================
//	==================== Solution of task 4 =================================
//	=========================================================================
	cout << "Task 4:" << endl;
	Mat img_gb;
	Mat img_f2D;
	Mat img_sepF2D;
	const double sigma = 2. * sqrt(2.);

	//  ====(a)==== 2D Filtering - using opencv "GaussianBlur()" ====
	tick = getTickCount();
	// TODO: implement your solution here:
	// ...
	GaussianBlur(bonn_gray, img_gb, Size(0, 0), sigma);

	tock = getTickCount();
	cout << "OpenCV GaussianBlur() method takes "
			<< (tock - tick) / getTickFrequency() << " seconds." << endl;

	//  ====(b)==== 2D Filtering - using opencv "filter2D()" ====
	// Compute gaussian kernel manually
	const int k_width = 3.5 * sigma;
	tick = getTickCount();
	Matx<float, 2 * k_width + 1, 2 * k_width + 1> kernel2D;
	for (int y = 0; y < kernel2D.rows; ++y) {
		const int dy = abs(k_width - y);
		for (int x = 0; x < kernel2D.cols; ++x) {
			const int dx = abs(k_width - x);
			//TODO: Fill kernel2D matrix with values of a gaussian
			// ...
			kernel2D(y, x) = exp(
					-0.5 * ((dx * dx + dy * dy)) / (sigma * sigma));
		}
	}
	kernel2D *= 1. / sum(kernel2D)[0];

	// TODO: implement your solution here - use "filter2D"
	// ...
	filter2D(bonn_gray, img_f2D, bonn_gray.depth(), kernel2D);

	tock = getTickCount();
	cout << "OpenCV filter2D() method takes "
			<< (tock - tick) / getTickFrequency() << " seconds." << endl;

	//  ====(c)==== 2D Filtering - using opencv "sepFilter2D()" ====
	tick = getTickCount();
	// TODO: implement your solution here
	Vec<float, 2 * k_width + 1> kernel1D;

	for (int y = 0; y < kernel1D.rows; ++y) {
		const int dy = abs(k_width - y);
		kernel1D[y] = exp(-0.5 * (dy * dy) / (sigma * sigma));
	}

	kernel1D *= 1. / sum(kernel1D)[0];

	sepFilter2D(bonn_gray, img_sepF2D, bonn_gray.depth(), kernel1D, kernel1D);

	tock = getTickCount();
	cout << "OpenCV sepFilter2D() method takes "
			<< (tock - tick) / getTickFrequency() << " seconds." << endl;

	// TODO: Show result images
	// ...
	imshow("Gaussian Blur", img_gb);
	imshow("filter2D", img_f2D);
	imshow("sepFilter2D", img_sepF2D);

//	imwrite("images/gaussianBlur.png", img_gb);
//	imwrite("images/filter2D.png", img_f2D);
//	imwrite("images/sepFilter2D.png", img_sepF2D);

	// compare blurring methods
	// TODO: Compute absolute differences between pixel intensities of (a), (b) and (c) using "absdiff"
	// ...

	// TODO: Find the maximum pixel error using "minMaxLoc"
	// ...
	absdiff(img_gb, img_f2D, diff);
	minMaxLoc(diff, &minVal, &maxVal);
	cout << "GaussianBlur() against filter2D() maximum pixel error: " << maxVal
			<< endl;

	absdiff(img_gb, img_sepF2D, diff);
	minMaxLoc(diff, &minVal, &maxVal);
	cout << "GaussianBlur() against sepFilter2D() maximum pixel error: "
			<< maxVal << endl;

	absdiff(img_f2D, img_sepF2D, diff);
	minMaxLoc(diff, &minVal, &maxVal);
	cout << "filter2D() against sepFilter2D() maximum pixel error: " << maxVal
			<< endl;

	waitKey(0);
	destroyAllWindows();

//	=========================================================================
//	==================== Solution of task 6 =================================
//	=========================================================================
	cout << "Task 6:" << endl;
	//  ====(a)==================================================================
	// TODO: implement your solution here
	// ...
	//  ====(b)==================================================================
	// TODO: implement your solution here
	// ...

	//waitKey(0);
	//destroyAllWindows();

//	=========================================================================
//	==================== Solution of task 7 =================================
//	=========================================================================
	cout << "Task 7:" << endl;
	// Create an image with salt and pepper noise
	Mat bonn_salt_pepper(bonn_gray.size(), bonn_gray.type());
	randu(bonn_salt_pepper, 0, 100); // Generates an array of random numbers
	for (int y = 0; y < bonn_gray.rows; ++y) {
		for (int x = 0; x < bonn_gray.cols; ++x) {
			uchar& pix = bonn_salt_pepper.at<uchar>(y, x);
			if (pix < 15) {
				// TODO: Set set pixel "pix" to black
				pix = 0;
			} else if (pix >= 85) {
				// TODO: Set set pixel "pix" to white
				pix = 255;
			} else {
				// TODO: Set set pixel "pix" to its corresponding intensity value in bonn_gray
				pix = bonn_gray.at<uchar>(y, x);
			}
		}
	}
	imshow("bonn.png with salt and pepper", bonn_salt_pepper);
//	imwrite("images/salt.png", bonn_salt_pepper);
	//  ====(a)==================================================================
	// TODO: implement your solution here
	Mat res_gb;
	GaussianBlur(bonn_salt_pepper, res_gb, Size(0, 0), sigma);
	imshow("GaussianBlur on Salt and Pepper", res_gb);
//	imwrite("images/gb_on_salt.png", res_gb);
	//  ====(b)==================================================================
	// TODO: implement your solution here
	Mat res_med;
	medianBlur(bonn_salt_pepper, res_med, 5);
	imshow("Median Blur on Salt and Pepper", res_med);
//	imwrite("images/mb_on_salt.png", res_med);
	//  ====(c)==================================================================
	// TODO: implement your solution here
	Mat res_bil;
	bilateralFilter(bonn_salt_pepper, res_bil, -1, 500, 5);
	imshow("bilateral filter on Salt and Pepper", res_bil);
//	imwrite("images/bilat_on_salt.png", res_bil);

	waitKey(0);
	destroyAllWindows();
//	=========================================================================
//	==================== Solution of task 8 =================================
//	=========================================================================
	cout << "Task 8:" << endl;
	// Declare Kernels
	Mat kernel1 =
			(Mat_<float>(3, 3) << 0.0113, 0.0838, 0.0113, 0.0838, 0.6193, 0.0838, 0.0113, 0.0838, 0.0113);
	Mat kernel2 =
			(Mat_<float>(3, 3) << -0.8984, 0.1472, 1.1410, -1.9075, 0.1566, 2.1359, -0.8659, 0.0573, 1.0337);

	//  ====(a)==================================================================
	// TODO: implement your solution here
	Mat res_filter2D_k1, res_filter2D_k2;
	filter2D(bonn_gray, res_filter2D_k1, bonn_gray.depth(), kernel1);
	filter2D(bonn_gray, res_filter2D_k2, bonn_gray.depth(), kernel2);
	imshow("filter2D Kernel 1 output", res_filter2D_k1);
	imshow("filter2D Kernel 2 output", res_filter2D_k2);

//	imwrite("images/filter2D_k1.png", res_filter2D_k1);
//	imwrite("images/filter2D_k2.png", res_filter2D_k2);
	//  ====(b)==================================================================
	// TODO: implement your solution here
	SVD svd1(kernel1);

	//get highest singular value
	float high_sing_val = sqrt(svd1.w.at<float>(0, 0));
	Mat filterV = svd1.u.col(0) * high_sing_val;
	Mat filterH = svd1.vt.row(0) * high_sing_val;

	Mat res_sepFilter_k1;
	sepFilter2D(bonn_gray, res_sepFilter_k1, bonn_gray.depth(), filterH,
			filterV);
	imshow("sepFilter kernel 1", res_sepFilter_k1);
//	imwrite("images/sepfilter2D_k1.png", res_sepFilter_k1);

	SVD svd2(kernel2);

	//get highest singular value
	float high_sing_val2 = sqrt(svd2.w.at<float>(0, 0));
	Mat filterV2 = svd2.u.col(0) * high_sing_val2;
	Mat filterH2 = svd2.vt.row(0) * high_sing_val2;

	Mat res_sepFilter_k2;
	sepFilter2D(bonn_gray, res_sepFilter_k2, bonn_gray.depth(), filterH2,
			filterV2);
	imshow("sepFilter kernel 2", res_sepFilter_k2);
//	imwrite("images/sepfilter2D_k2.png", res_sepFilter_k2);
	//  ====(c)==================================================================
	// TODO: implement your solution here
	// Compute the absolute pixelwise difference
	Mat matAbsDiff_kern1, matAbsDiff_kern2;
	absdiff(res_filter2D_k1, res_sepFilter_k1, matAbsDiff_kern1);
	absdiff(res_filter2D_k2, res_sepFilter_k2, matAbsDiff_kern2);

	// Display the maximum pixel error
	minMaxLoc(matAbsDiff_kern1, &minVal, &maxVal);
	cout << "kernel1 2D vs kernel1 separated, maximum pixel error: " << maxVal
			<< endl;
	cout << "The error here is small since kernel 1 was a Gaussian and therefore lineary separabel"
			<< endl;

	minMaxLoc(matAbsDiff_kern2, &minVal, &maxVal);
	cout << "kernel2 2D vs kernel2 separated, maximum pixel error: " << maxVal
			<< endl;
	cout << "This error is comparably high since kernel 2 was not lineary separabel"
			<< endl;

	cout << "Program finished successfully" << endl;
	waitKey(0);
	destroyAllWindows();
	return 0;
}

