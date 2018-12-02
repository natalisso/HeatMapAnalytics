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

class Chess {
public:
	int id;
	string name;
	Point2f center;
	std::vector<cv::Point2f> trackingPoints;

	Point2f findCenter(Point2f, Point2f);
	void drawTrackingPath(cv::Mat, int, Chess*);
};


Point2f Chess::findCenter(Point2f point1, Point2f point2) {
	Point2f retCenter;
	retCenter.x = abs(point2.x - point1.x) / 2;
	retCenter.y = abs(point2.y - point1.y) / 2;

	return retCenter;
}

void Chess::drawTrackingPath(cv::Mat imageDraw, int id, Chess *chessPieces) {
	string text;
	int font = FONT_HERSHEY_SIMPLEX;
	double fontScale = 1;
	int thickness = 1;
	size_t vectorSize = chessPieces[id].trackingPoints.size();
	int R, G, B;

	if (id >= 0 || id <= 15) {
		R = 0;
		B = 0;
		G = 0;
	}
	else {
		R = 255;
		B = 255;
		G = 255;
	}

	for (int i = 1; i < vectorSize; i++) {
		if (i == 0)
			putText(imageDraw, "Beginning", chessPieces[id].trackingPoints[i], font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
		else if (i == vectorSize - 1)
			putText(imageDraw, "End", chessPieces[id].trackingPoints[i], font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
		cv::line(imageDraw, chessPieces[id].trackingPoints[i - 1], chessPieces[id].trackingPoints[i], cv::Scalar(B, G, R), 1, LINE_AA, 0);
	}
}

void generateMarkers();
void initializeMarkers();
void drawMyMarkers(cv::Mat, std::vector<std::vector<cv::Point2f> >, std::vector<int>);
void trackingMarkers(Chess*, Point2f);
void endGame(cv::Mat);

//Chess chessPieces[32];
Chess chessPieces;

int main(){
	cv::Mat image, imageCopy, imageCopy2;
	std::vector<int> ids;
	std::vector<std::vector<cv::Point2f> > corners;
	Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);
	int flag = 1, option = -1;


	//GERANDO OS MARCADORES
	//generateMarkers();

	//INICIALIZANDO O VETOR COM AS PEÇAS DO XADREZ
	initializeMarkers();

	//DETECTANDO OS MARCADORES
	cv::VideoCapture inputVideo;
	inputVideo.open(0);

	while (inputVideo.grab()) {
		
		// Grava a imagem do frame atual e copia para imageCopy (para poder fazer modificações)
		inputVideo.retrieve(image);
		image.copyTo(imageCopy);
		if (flag) {
			image.copyTo(imageCopy2);
			flag = 0;
		}

		// Detecta todas os marcadores retornando a posição 2D dos 4 cantos (sup esq., sup dir., inf dir., inf esq.) e a id dele
		cv::aruco::detectMarkers(image, markerDictionary, corners, ids);

		// Se detectou ao menos 1 marcador, mostra a detecção na imagem
		if (ids.size() > 0)
			drawMyMarkers(imageCopy, corners, ids);
		cv::imshow("out", imageCopy);
		char key = (char)cv::waitKey(30);
		if (key == 27) {
			endGame(imageCopy2);
			break;		
		}
	}

	return 0;
}


void generateMarkers() {
	// Cria os marcadores e salva em arquivos .jpg
	int i;
	cv::Mat outputMarker;
	Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);

	for (i = 0; i < 50; i++)
	{
		aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
		ostringstream convert;
		string imageName = "5x5Marker_";
		convert << imageName << i << ".jpg";
		cv::imwrite(convert.str(), outputMarker);
	}
}

void drawMyMarkers(cv::Mat imageCopy, std::vector<std::vector<cv::Point2f> > corners, std::vector<int> ids) {
	cv::Point2f newCenter;
	string text;
	int font = FONT_HERSHEY_SIMPLEX;
	double fontScale = 1;
	int thickness = 1;

	for (int i = 0; i < corners.size(); i++) {
		cv::rectangle(imageCopy, corners[i][0], corners[i][2], cv::Scalar(0, 255, 0));
		//newCenter = chessPieces[ids[i]].findCenter(corners[i][0], corners[i][2
		//putText(imageCopy, chessPieces[ids[i]].name, newCenter, font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
		//trackingMarkers(&chessPieces[ids[i]], newCenter);
		newCenter = chessPieces.findCenter(corners[i][0], corners[i][2]);
		putText(imageCopy, chessPieces.name, newCenter, font, fontScale, cv::Scalar(0, 0, 255), thickness, LINE_AA);
		trackingMarkers(&chessPieces, newCenter);	
	}	
}

void trackingMarkers(Chess* currentPiece, Point2f newCenter) {
	size_t vectorSize = 0;

	vectorSize = currentPiece->trackingPoints.size();
	if (vectorSize == 0)
		currentPiece->trackingPoints.emplace_back(newCenter);
	else {
		if (abs(currentPiece->trackingPoints[vectorSize - 1].x - newCenter.x) > 20
			|| abs(currentPiece->trackingPoints[vectorSize - 1].y - newCenter.y) > 20) {
			//pontosTracking.emplace_back(newCenter);
			currentPiece->trackingPoints.emplace_back(newCenter);
			printf("new:(%.3f, %.3f)  old:(%.3f, %.3f)\n", newCenter.x, newCenter.y, currentPiece->trackingPoints[vectorSize - 1].x, currentPiece->trackingPoints[vectorSize - 1].y);
			//trackingMarkers(imageCopy2, chessPieces[i].trackingPoints, ids[i], newCenter);
			//cv::line(imageCopy2, oldCenter, newCenter, cv::Scalar(255, 0, 0), 1, LINE_AA, 0);

			//oldCenter = newCenter;
		}
	}
}

void endGame(cv::Mat imageCopy2) {
	int option = -1;

	printf("Choose a chess piece (4 and 20 -> King) or 32 to exit:\n");
	printf("     BLACK     \n");
	printf("0 1 2 3 4 5 6 7\n8 9 10 11 12 13 14 15\n\n\n\n\n\n");
	printf("     WHITE     \n");
	printf("24 25 26 27 28 29 30 31\n16 17 18 19 20 21 22 23\n");
	do {
		scanf("%d", &option);
	} while (option < 0 || option > 33);

	if (option != 33){
		//while (1) {
		//chessPieces[option].drawTrackingPath(imageCopy2, (option), chessPieces
		chessPieces.drawTrackingPath(imageCopy2, (option), &chessPieces);
		//cv::imshow("TrackingPath", imageCopy2);

		//char key = (char)cv::waitKey(30);
		//if (key == 27) {
		ostringstream convert;
		string imageName = "TrackingPath_";
		//convert << imageName << chessPieces[option].name << ".jpg";
		convert << imageName << chessPieces.name << ".jpg";
		cv::imwrite(convert.str(), imageCopy2);
		//break;
			//}
		//}
	}
}

void initializeMarkers() {
	/*for (int i = 0; i < 32; i++) {
		chessPieces[i].id = i;
		chessPieces[i].center.x = -1;
		chessPieces[i].center.y = -1;
		if (i%16 == 0 || i%16 == 7) 
			chessPieces[i].name = "Rook";
		else if(i%16 == 1 || i%16 == 6)
			chessPieces[i].name = "Knight";
		else if (i % 16 == 2 || i%16 == 5)
			chessPieces[i].name = "Bishop";
		else if (i % 16 == 3)
			chessPieces[i].name = "Queen";
		else if (i % 16 == 4)
			chessPieces[i].name = "King";
		else
			chessPieces[i].name = "Pawn";
	}*/
	chessPieces.id = 0;
	chessPieces.center.x = -1;
	chessPieces.center.y = -1;
	chessPieces.name = "Rook";
}