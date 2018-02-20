#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat smallcleanup(Mat binary, int n) {
	Mat input = binary.clone();
	Mat cleaned = binary.clone();
	Mat markers = Mat::zeros(binary.rows, binary.cols, CV_8U);
	vector<vector<Point> > contours;
	findContours(input, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(255), -1);
		if (countNonZero(markers) < n) {
			drawContours(cleaned, contours, static_cast<int>(i), Scalar::all(0), -1);
		}
		markers = Mat::zeros(binary.rows, binary.cols, CV_8U);
	}
	return cleaned;
}

Mat bigcleanup(Mat binary, int n) {
	Mat input = binary.clone();
	Mat cleaned = binary.clone();
	Mat markers = Mat::zeros(binary.rows, binary.cols, CV_8U);

	vector<vector<Point> > contours;
	findContours(input, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(255), -1);

		if (countNonZero(markers) > n) {
			drawContours(cleaned, contours, static_cast<int>(i), Scalar::all(0), -1);
		}
		markers = Mat::zeros(binary.rows, binary.cols, CV_8U);
	}
	return cleaned;
}


Mat fill(Mat binary) {
	Mat input = binary.clone();
	Mat filled = Mat::zeros(binary.rows, binary.cols, CV_8U);
	vector<vector<Point> > contours;
	findContours(input, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(filled, contours, static_cast<int>(i), Scalar::all(255), -1);
	}
	return filled;
}

