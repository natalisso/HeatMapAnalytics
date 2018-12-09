#pragma once
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class MyChessPiece
{
public:
	int id, B, G, R;					// A cor da peça é armazenada em B,G,R 
	String name;						// Nome da peça do xadrez
	Point2f center;						// Armazena o centro do marcador
	vector<Point2f> trackingPoints;		// Armazena os pontos onde o centro da peça esteve

	MyChessPiece();
	MyChessPiece(int PieceId);
	void findCenter(Point2f point1, Point2f point2);		// Recálculo do centro da peça
	void storeTrackingPoints();								// Guarda os pontos do centro dos marcadores encontrados
	void drawHeatMap(Mat& imageBackground);					// Liga os pontos armazenados no vetor trackingPoints e salva em uma imagem
};

