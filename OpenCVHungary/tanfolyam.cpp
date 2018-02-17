//Ha kérdésed van írj bátran: nhajni@gmail.com

#include <opencv2/opencv.hpp> //Ez kell, hogy működjön az opencv
#include <iostream> //Konzolra írás és beolvasás
#include <string> //Stringek kezelése

//Így nem kell mindig kiírni, hogy cv:: és std::
using namespace cv;
using namespace std;

//Ha kerestek bármilyen opencv dologra, keresőbe írjátok be, hogy opencv 3 c++ (opencv 2-es megoldások nem mindig működnek),
//ha példát szeretnétek hozzá még írjátok mögé, hogy example sokat szokott segíteni

int main() {
	/*Kép beolvasása Mat fájlba
	http://docs.opencv.org/3.0-beta/modules/imgcodecs/doc/reading_and_writing_images.html
	-src szokott lenne a source fájl (időnként Mat img, vagy Mat image)
	-nem kell open/close
	-*/

	//Infók Lénáról :) 
	//https://hu.wikipedia.org/wiki/Lenna_(f%C3%A9nyk%C3%A9p)
	Mat src = imread("lena.tif", -1);

	//Ellenőrizzük, hogy sikerült-e beolvasni a képet SOSE SPÓROLJÁTOK LE!
	if (!src.data) {
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	//Típus 
	//Mat típusának lekérése, nagyon fontos, mert a különböző képfeldogozó algoritmusok csak bizonyos típusú képekre működnek
	//http://ninghang.blogspot.hu/2012/11/list-of-mat-type-in-opencv.html
	cout << "Type of src is : " << src.type() << endl; //CV_8UC3
	cout << "Size of src is : " << src.size() << endl;
	//imshow("Original", src);

	//Sávokra bontás
	//http://answers.opencv.org/question/59529/how-do-i-separate-the-channels-of-an-rgb-image-and-save-each-one-using-the-249-version-of-opencv/
	Mat bgr[3];
	split(src, bgr);
	Mat red = bgr[2];
	Mat green = bgr[1];
	Mat blue = bgr[0];
	Mat gray;
	cvtColor(src,gray,CV_RGB2GRAY);
	//Kép megjelenítése
	//imshow("Red", red);
	//imshow("Green", green);
	//imshow("Blue", blue);
	//imshow("Gray", gray);

	//Tükrözés
	//https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#void%20flip(InputArray%20src,%20OutputArray%20dst,%20int%20flipCode)
	Mat fliped;
	flip(gray, fliped,1);
	//imshow("Flip", fliped);

	//Range - min-max keresés
	double min, max;
	minMaxLoc(gray, &min, &max);
	
	//http://answers.opencv.org/question/27248/max-and-min-values-in-a-mat/
	cout << "Range gray: " << min << " " << max << " mean: " << mean(gray) << endl;

	//Hisztogramm kiegyenlítés
	//https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_equalization/histogram_equalization.html
	Mat gray_eq;
	equalizeHist(gray, gray_eq);
	//imshow("Gray_eq", gray_eq);
	minMaxLoc(gray_eq, &min, &max);
	cout << "Range gray_eq: " << min << " " << max  << " mean: " << mean(gray) <<endl;
	

	//Kép küszöbölése http://docs.opencv.org/3.1.0/db/d8e/tutorial_threshold.html
	//Sok művelethez bináris kép kell bemenetként, így állíthatunk elő ilyeneket
	Mat dst;
	threshold(gray_eq, dst, 128, 255, 0);
	//imshow("Thresholded", dst);
	minMaxLoc(gray_eq, &min, &max);
	cout << "Range tresholded gray_eq: " << min << " " << max << " mean: " << mean(dst) << endl;
	
	//Kép simítása
	//https://docs.opencv.org/3.0-beta/modules/imgproc/doc/filtering.html
	Mat kernel = Mat::ones(3, 3, CV_32F) / 9.0;
	//cout << kernel << endl;
	Mat filtered;
	int n = 10; //Hányszor simítsuk el a képet, jobb egy képet többször kis szűrővel simítani, mint kevésszer nagy szűrőket használni

	for (int i = 0; i < n; i++) {
		filter2D(gray, filtered, -1, kernel);
		//imshow("Smoothed", filtered);
		minMaxLoc(filtered, &min, &max);
		//cout << "Range filtered: " << min << " " << max << " mean: " << mean(filtered) << endl;
		gray = filtered;
		
	}

	//Környezettől való eltérés
	Mat kernel1 = (Mat_<int>(3, 3) << 0, 0, 0, 0, 2, 0, 0, 0, 0);
	Mat filtered2;
	filter2D(gray, filtered2, -1, kernel1);
	Mat elter = filtered2 - filtered;
	//imshow("Elter", elter);

	//Sobel és Scharr
	//https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html
	Mat grad;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr(gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr(gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	imshow("Grad", grad);
	cout << "Range grad: " << min << " " << max << " mean: " << mean(gray) << endl;
	threshold(grad, dst, 25, 255, 0);
	imshow("Thresholded", dst);

	//Canny éldetektor
	//http://docs.opencv.org/3.1.0/dd/d1a/group__imgproc__feature.html#ga04723e007ed888ddf11d9ba04e2232de
	//http://docs.opencv.org/3.1.0/d3/d63/edge_8cpp-example.html#a3
	Mat edge;
	double edgeThresh = 40;
	Canny(dst, edge, edgeThresh, edgeThresh * 3);
	imshow("Canny", edge);
	waitKey();

	return 0;
}