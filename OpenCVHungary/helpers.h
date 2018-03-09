#pragma once
#include <opencv2/opencv.hpp>
#include <chrono>

namespace cv {
	void imshowResized(const cv::String& winname, const cv::Mat& img);
}

extern class Timer {

private:
	const std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> middleTime, cinStartTime;
	std::chrono::duration<double, std::milli> cinDuration;

public:
	/**
	Elindítja az idõmérést.
	*/
	Timer();

	/**
	Kiírja a timer példányosítása, vagy a midTime/cinEnd függvények meghívása óta eltelt idõt.
	Az egyes részfeladatok részidejének mérésére alkalmas.

	@return Az eltelt idõ milliszekundumban, zárójelben.
	*/
	cv::String midTime();

	/**
	Feljegyzi az idõt az input bekérés elõtt.
	Egymás utáni, cinEnd nélküli többszöri meghívás esetén az idõt csak az elsõ futáskor jegyzi fel.
	*/
	void cinStart();

	/**
	Input bekérés után megméri a bekérés eleje óta eltelt idõt, és hozzáadja a teljes input idõtartamhoz.
	Újraindítja a részidõ mérését.
	*/
	void cinEnd();

	/**
	A program teljes futásidejébõl kivonja a teljes input idõtartamot, majd az eredményt kiírja.
	*/
	void finish();
};