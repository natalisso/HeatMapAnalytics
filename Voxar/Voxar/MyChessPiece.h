#pragma once
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class MyChessPiece
{
public:
	int id, B, G, R;
	String name;
	Point2f center;
	vector<Point2f> trackingPoints;

	MyChessPiece();
	MyChessPiece(int PieceId);
	void findCenter(Point2f point1, Point2f point2);
	void storeTrackingPoints();															// Guarda os pontos do centro dos marcadores encontrados
	void drawHeatMap(Mat& imageBackground);
};

