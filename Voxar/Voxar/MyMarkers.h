#pragma once
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\aruco.hpp"
#include "MyChessPiece.h"
#include <iostream>

using namespace std;
using namespace cv;
using namespace aruco;

class MyMarkers
{
public:
	Ptr<Dictionary> markerDictionary;
	vector<vector<Point2f>> corners;
	vector<int> ids;

	MyMarkers();
	void generateMarkers(); 															// Cria os marcadores e salva em arquivos .jpg
	void drawMyMarkers(Mat& currentFrame, vector<MyChessPiece>& allChessPieces);		// Desenha um retângulo em volta do marcado detectado junto com o nome da peça que ele representa
};

