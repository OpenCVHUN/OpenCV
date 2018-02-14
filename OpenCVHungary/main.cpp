#include <opencv2/opencv.hpp>

int main()
{
	cv::Mat whiteRect(500, 500, CV_8UC1, cv::Scalar(255));
	cv::imshow("Test", whiteRect);
	cv::waitKey();

	return 0;
}