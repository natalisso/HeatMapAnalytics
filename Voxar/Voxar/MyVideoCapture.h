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
	Mat frame;
	VideoCapture inputVideo;
	clock_t startTime;
	double fps, frameCounter, totalTime;
	int source;
	bool stop;			// Uma variável de parada para comunicação com a main

	MyVideoCapture(int sourceOption);
	void getFrame();					// Captura um frame e da o release na captura quando o loop na main para
	void beginTimer();					// Inicial o tempo
	void updateFPS();					// Incrementa a quantidade de frames capturados, o tempo total de execução e o a quantidade de fps
	void printFPS(Mat& image);			// Imprime no frame atual o fps 
};






