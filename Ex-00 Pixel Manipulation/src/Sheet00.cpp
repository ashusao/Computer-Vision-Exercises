/*
 * Sheet00.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: ashu
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;

void readImage(const char* file, Mat& result) {
	//TODO: implement your solution here
	result = imread(file, IMREAD_COLOR);
}

void display(const char* windowTitle, Mat& img) {
	//TODO: implement your solution here
	namedWindow(windowTitle, WINDOW_AUTOSIZE);
	imshow(windowTitle, img);
	waitKey(0);
//	string file = "images/" +  string(windowTitle) + ".png";
//	imwrite( file, img );
}

void convertToGrayImg(Mat& img, Mat& result) {
	//TODO: implement your solution here
	cvtColor(img, result, COLOR_BGR2GRAY);
}

void subtractIntensityImage(Mat& bgrImg, Mat& grayImg, Mat& result) {
	//TODO: implement your solution here
	vector<Mat> channels;
	split(bgrImg, channels);
	for (unsigned int i = 0; i < channels.size(); ++i) {
		subtract(channels[i], 0.5 * grayImg, channels[i]);
		channels[i] = max(channels[i], 0);
	}
	merge(channels, result);

}

void pixelwiseSubtraction(Mat& bgrImg, Mat& grayImg, Mat& result) {
	//TODO: implement your solution here

	int channels = bgrImg.channels();
	int nrows = bgrImg.rows;
	int ncols = bgrImg.cols * channels;

	// if continous make it one complete row
	if (bgrImg.isContinuous()) {
		ncols = ncols * nrows;
		nrows = 1;
	}

	bgrImg.copyTo(result);

	for (int i = 0; i < nrows; i++) {
		uchar *p = result.ptr<uchar>(i);//take pointer to first element in row from result Mat
		uchar *q = grayImg.ptr<uchar>(i);// take pointer to first Element in row from grayscale image
		for (int j = 0, k = 0; j < ncols; j++) {
			if (j % 3 == 0)	// increment grayscale pointer once after bgr pointer is incremented thrice
				k++;
			p[j] = max(p[j] - 0.5 * q[k], 0.0);
		}
	}

}

void extractPatch(Mat& img, Mat& result) {
	//TODO: implement your solution here
	int pSize = 16;
	Rect ROI(img.cols / 2 - pSize / 2, img.rows / 2 - pSize / 2, pSize, pSize);
	Mat patch = img(ROI);
	patch.copyTo(result);

}

void copyPatchToRandomLocation(Mat& img, Mat& patch, Mat& result) {
	//TODO: implement your solution here
	RNG rng(100);
	int pSize = 16;
	img.copyTo(result);
	Point P(rng.uniform(0, img.cols - pSize), rng.uniform(0, img.rows - pSize));

	Rect roi(P.x, P.y, pSize, pSize);
	Mat dest = result(roi);		// returns the pointer to location
	patch.copyTo(dest);	// copies the patch to destination location

}

void drawRandomRectanglesAndEllipses(Mat& img) {
	//TODO: implement your solution here
	RNG rng(100);

	int maxSize = 50;

	for (int i = 0; i < 10; i++) {
		Point pt1(rng.uniform(0, img.cols - maxSize),
				rng.uniform(0, img.rows - maxSize));
		Point pt2(rng.uniform(0, img.cols - maxSize),
				rng.uniform(0, img.rows - maxSize));
		Scalar col1 = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		rectangle(img, pt1, pt2, col1, 2);

		Point center(rng.uniform(0, img.cols - maxSize),
				rng.uniform(0, img.rows - maxSize));
		Size sz(rng.uniform(0, maxSize), rng.uniform(0, maxSize));
		double angle = rng.uniform(0, 360);

		ellipse(img, center, sz, angle, 0, 360, col1, 2);

	}

}

int main(int argc, char* argv[]) {
	// check input arguments
	if (argc != 2) {
		cout << "usage: " << argv[0] << " <path_to_image>" << endl;
		return -1;
	}

	// read and display the input image
	Mat bgrImg;
	readImage(argv[1], bgrImg);
	display("(a) inputImage", bgrImg);

	// (b) convert bgrImg to grayImg
	Mat grayImg;
	convertToGrayImg(bgrImg, grayImg);
	display("(b) grayImage", grayImg);

	// (c) bgrImg - 0.5*grayImg
	Mat imgC;
	subtractIntensityImage(bgrImg, grayImg, imgC);
	display("(c) subtractedImage", imgC);

	// (d) pixelwise operations
	Mat imgD;
	pixelwiseSubtraction(bgrImg, grayImg, imgD);
	display("(d) subtractedImagePixelwise", imgD);

	// (e) copy 16x16 patch from center to a random location
	Mat patch;
	extractPatch(bgrImg, patch);
	display("(e) random patch", patch);

	Mat imgE;
	copyPatchToRandomLocation(bgrImg, patch, imgE);
	display("(e) afterRandomCopying", imgE);

	// (f) drawing random rectanges on the image
	drawRandomRectanglesAndEllipses(bgrImg);
	display("(f) random elements", bgrImg);

	return 0;
}

