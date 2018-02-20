#include <iostream> 
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace std;
using namespace cv;

int main() {
	char command[200];
	//snprintf(command, sizeof(command), "C:\\lastools\\bin\\lasview.exe -i craters.las");
	snprintf(command, sizeof(command), "C:\\lastools\\bin\\las2dem -i craters.las -elevation -otif");
	system(command);

	Mat dtm = imread("craters.tif", -1);
	if (!dtm.data) {
		cout << "No input dtm" << endl;
		return -1;
	}

	//cout << dtm.size();
		
	double min, max;
	Point min_loc, max_loc;
	minMaxLoc(dtm, &min, &max, &min_loc, &max_loc);
	cout << "Min: " << min << " max:" << max <<  endl;
	//cout << src << endl;

	Mat smoothed;
	Mat kernel = Mat::ones(3, 3, CV_32F) / (float)(3*3);
	//cout << kernel;
	for (int i = 0; i <= 30; i++) {
		filter2D(dtm, smoothed, CV_32F, kernel);
	}
	
	Mat diff;
	diff = smoothed - dtm;
	imshow("1. Diff", diff);

	Mat binary;
	double breakvalue = 0.05;  // m-ben
	threshold(diff, binary, breakvalue, 255, 0);
	if (binary.type() != CV_8U) {
		binary.convertTo(binary, CV_8U);
	}
	imshow("2. Binary", binary);

	int small = 30;
	Mat cleaned1 = smallcleanup(binary, small);
	imshow("3. Cleaned1", cleaned1);

	int big = 50;
	Mat cleaned2 = bigcleanup(cleaned1, big);
	imshow("4. Cleaned2", cleaned2);

	Mat closed;
	Mat mkernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(cleaned2, closed, MORPH_CLOSE, mkernel, Point(-1, -1), 1);
	imshow("5. Closed", closed);

	Mat filled = fill(closed);
	imshow("6. Filled", filled);

	Mat opened;
	morphologyEx(filled, opened, MORPH_OPEN, mkernel, Point(-1, -1), 2);
	imshow("7. Opened", opened);

	Mat ref = imread("reference.tif", -1);
	imshow("8. Reference", ref);
	//cout << ref.size();

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(opened, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	cvtColor(ref, ref, CV_GRAY2BGR);
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		Scalar color = Scalar(255, 0, 255);
		circle(ref, mc[i], 0, color, 5, 0, 0);
	}

	imshow("9. Solution", ref);

	waitKey(0);
	return 0;
}