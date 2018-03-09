#include <opencv2/opencv.hpp>
#include <wtypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "helpers.h"

void cv::imshowResized(const cv::String& winname, const cv::Mat& img) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	double screenWidth = desktop.right;
	double screenHeight = desktop.bottom;
	int horRatio, verRatio, zoomRatio;

	if (screenWidth < img.cols || screenHeight < img.rows) {
		horRatio = ceil(img.cols / screenWidth);
		verRatio = ceil(img.rows / screenHeight);
	}
	else {
		horRatio, verRatio = 1;
	}
	zoomRatio = max(horRatio, verRatio);

	cv::namedWindow(winname, cv::WINDOW_NORMAL);
	cv::resizeWindow(winname, img.cols / zoomRatio, img.rows / zoomRatio);
	cv::imshow(winname, img);
}

Timer::Timer() : startTime(std::chrono::high_resolution_clock::now()), middleTime(std::chrono::high_resolution_clock::now()) {}

cv::String Timer::midTime() {
	std::chrono::duration<double, std::milli> midDuration(std::chrono::high_resolution_clock::now() - middleTime);
	std::ostringstream ostr;
	ostr << std::fixed << std::setprecision(2) << midDuration.count();
	middleTime = std::chrono::high_resolution_clock::now();

	return "(" + ostr.str() + " ms)";
}

void Timer::cinStart() {
	if (cinStartTime == std::chrono::time_point<std::chrono::steady_clock>()) {
		cinStartTime = std::chrono::high_resolution_clock::now();
	}
}

void Timer::cinEnd() {
	cinDuration += std::chrono::high_resolution_clock::now() - cinStartTime;
	cinStartTime = std::chrono::time_point<std::chrono::steady_clock>();
	middleTime = std::chrono::high_resolution_clock::now();
}

void Timer::finish() {
	std::chrono::duration<double> runDuration(std::chrono::high_resolution_clock::now() - startTime - cinDuration);
	std::ostringstream ostr;
	ostr << std::fixed << std::setprecision(3) << runDuration.count();
	std::cout << std::endl << "Futasido: " << ostr.str() << " masodperc" << std::endl << std::endl;
}