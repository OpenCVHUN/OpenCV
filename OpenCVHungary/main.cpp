#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lena.tif", -1);
	if (!img.data) {
		cout << "Could not find or open lena.tif" << endl;
		return -1;
	}
	waitKey(0);
	return 0;
}