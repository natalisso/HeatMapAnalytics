#pragma once
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;
using namespace cv;

class MyVideoCapture
{
public:
	Mat frame, initialFrame;
	VideoCapture inputVideo;
	clock_t startTime;
	double fps, frameCounter, totalTime;
	int source;
	bool stop;

	MyVideoCapture(int sourceOption);
	void getInitialFrame();
	void getFrame();
	void showFrame();
	void beginTimer();
	void updateFPS();
	void printFPS(Mat& image);
};






