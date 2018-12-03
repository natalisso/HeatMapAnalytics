#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\aruco.hpp"
#include "opencv2\calib3d.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
using namespace aruco;

class Chess {
public:
	int id;
	char name[7];
	Point2f center;
	vector<Point2f> trackingPoints;

	Point2f findCenter(Point2f, Point2f);
	void drawTrackingPath(Mat&, int, vector<Chess>&);
};


Point2f Chess::findCenter(Point2f point1, Point2f point2) {
	Point2f retCenter;
	if (point1.x < point2.x) 
		retCenter.x = point1.x + (abs(point2.x - point1.x) / 2);
	else
		retCenter.x = point2.x + (abs(point2.x - point1.x) / 2);
	if(point1.y < point2.y)
		retCenter.y = point1.y +(abs(point2.y - point1.y) / 2);
	else
		retCenter.y = point2.y + (abs(point2.y - point1.y) / 2);
	this->center = retCenter;

	return retCenter;
}

void Chess::drawTrackingPath(Mat& imageDraw, int id, vector<Chess>& chessPieces) {
	int font = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.8;
	int thickness = 1.5;
	size_t vectorSize = chessPieces[id].trackingPoints.size();
	int R, G, B;

	if (id >= 0 || id <= 15) {
		R = 0;
		B = 255;
		G = 0;
	}
	else {
		R = 0;
		B = 0;
		G = 255;
	}

	for (int i = 1; i < vectorSize; i++) {
		line(imageDraw, chessPieces[id].trackingPoints[i - 1], chessPieces[id].trackingPoints[i], Scalar(B, G, R), 2, LINE_AA, 0);
		if (i == 1)
			circle(imageDraw, chessPieces[id].trackingPoints[i - 1], 4, Scalar(B, G, R), 4, LINE_AA, 0);
		else if (i == vectorSize - 1)
			putText(imageDraw, "End", chessPieces[id].trackingPoints[i], font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
	}
}

void generateMarkers(); 															// Cria os marcadores e salva em arquivos .jpg
void initializeMarkers(vector<Chess>&);												// Inicializa o vetor de peças no formato certo
void drawMyMarkers(Mat&, vector<vector<Point2f>>&, vector<int>&, vector<Chess>&);	// Desenha um retângulo em volta do marcado detectado junto com o nome da peça que ele representa
void trackingMarkers(Chess&, Point2f);												// Guarda os pontos do centro dos marcadores encontrados
void endGame(Mat&, vector<Chess>&);													// Salva a imagem com o tracking path dos marcadores		


int main(){
	Mat image, imageCopy, imageCopy2;
	vector<Chess> chessPieces;
	vector<int> ids;
	vector<vector<Point2f> > corners;
	Ptr<Dictionary> markerDictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);
	int flag = 1;
	
	//GERANDO OS MARCADORES
	//generateMarkers();

	//INICIALIZANDO O VETOR COM AS PEÇAS DO XADREZ
	initializeMarkers(chessPieces);

	//DETECTANDO OS MARCADORES
	VideoCapture inputVideo;
	inputVideo.open(1);

	while (inputVideo.grab()) {
		
		// Grava a imagem do frame atual e copia para imageCopy (para poder fazer modificações)
		inputVideo.retrieve(image);
		image.copyTo(imageCopy);
		// Copia a imagem inicial para imageCopy2 (o trackingPath vai ser desenhado nela)
		if (flag) {
			image.copyTo(imageCopy2);
			flag = 0;
		}

		// Detecta todas os marcadores retornando a posição 2D dos 4 cantos (sup esq., sup dir., inf dir., inf esq.) e a id dele
		detectMarkers(image, markerDictionary, corners, ids);

		// Se detectou ao menos 1 marcador, mostra a detecção na imagem
		if (ids.size() > 0)
			drawMyMarkers(imageCopy, corners, ids, chessPieces);
		imshow("out", imageCopy);
		char key = (char)waitKey(30);
		if (key == 27) {
			endGame(imageCopy2, chessPieces);
			break;		
		}
	}

	return 0;
}


void generateMarkers() {
	Mat outputMarker;
	Ptr<Dictionary> markerDictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);

	for (int i = 0; i < 32; i++)
	{
		drawMarker(markerDictionary, i, 500, outputMarker, 1);
		ostringstream convert;
		string imageName = "5x5Marker_";
		convert << imageName << i << ".jpg";
		imwrite(convert.str(), outputMarker);
	}
}

void initializeMarkers(vector<Chess>& chessPieces) {
	char names[6][7] = { "Rook", "Knight", "Bishop", "Queen", "King", "Pawn" };
	int namesID;
	Chess currentPiece;
	Point2f center(-1.0, -1.0);

	for (int i = 0; i < 32; i++) {
		if (i % 16 == 0 || i % 16 == 7)
			namesID = 0;
		else if (i % 16 == 1 || i % 16 == 6)
			namesID = 1;
		else if (i % 16 == 2 || i % 16 == 5)
			namesID = 2;
		else if (i % 16 == 3)
			namesID = 3;
		else if (i % 16 == 4)
			namesID = 4;
		else
			namesID = 5;

		currentPiece.id = i;
		std::strcpy(currentPiece.name, names[namesID]);
		currentPiece.center = center;
		chessPieces.push_back(currentPiece);

		//printf("%s - %d (%.2f,%.2f)\n", chessPieces[i].name, chessPieces[i].id, chessPieces[i].center.x, chessPieces[i].center.y);
	}
}

void drawMyMarkers(Mat& imageCopy, vector<vector<Point2f>>& corners, vector<int>& ids, vector<Chess>& chessPieces) {
	Point2f newCenter, printCenter;
	string text;
	int font = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 1.5;
	int R, G, B;
	

	for (int i = 0; i < corners.size(); i++) {
		if (i >= 0 || i <= 15) {
			R = 0;
			B = 255;
			G = 0;
		}
		else {
			R = 0;
			B = 0;
			G = 255;
		}
		ostringstream convert;
		convert << "id: "<< chessPieces[ids[i]].id;
		rectangle(imageCopy, corners[i][0], corners[i][2], Scalar(B, G, R));
		newCenter = chessPieces[ids[i]].findCenter(corners[i][0], corners[i][2]);
		printCenter = chessPieces[ids[i]].findCenter(corners[i][0], newCenter);
		putText(imageCopy, chessPieces[ids[i]].name, printCenter, font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
		printCenter = chessPieces[ids[i]].findCenter(corners[i][3], newCenter);
		putText(imageCopy, convert.str(), printCenter, font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
		trackingMarkers(chessPieces[ids[i]], newCenter);
	}	
}

void trackingMarkers(Chess& currentPiece, Point2f newCenter) {
	size_t vectorSize = 0;

	vectorSize = currentPiece.trackingPoints.size();
	if (vectorSize == 0)
		currentPiece.trackingPoints.push_back(newCenter);
	else {
		if (abs(currentPiece.trackingPoints[vectorSize - 1].x - newCenter.x) > 20
			|| abs(currentPiece.trackingPoints[vectorSize - 1].y - newCenter.y) > 20) {
	
			currentPiece.trackingPoints.push_back(newCenter);
			//printf("ID: %d   new:(%.3f, %.3f)    old:(%.3f, %.3f)\n",currentPiece.id, newCenter.x, newCenter.y, currentPiece.trackingPoints[vectorSize - 1].x, currentPiece.trackingPoints[vectorSize - 1].y);

		}
	}
}

void endGame(Mat& imageCopy2, vector<Chess>& chessPieces) {
	for (int i = 0; i < 32; i++)
	{
		if (chessPieces[i].trackingPoints.size() > 0) {
			Mat imageFundo;
			imageCopy2.copyTo(imageFundo);
			chessPieces[i].drawTrackingPath(imageFundo, i, chessPieces);

			ostringstream convert;
			string imageName = "TrackingPath_";
			convert << imageName << chessPieces[i].name << "_" << chessPieces[i].id <<".jpg";
			imwrite(convert.str(), imageFundo);
		}
	}
}

