#include "MyVideoCapture.h"
#include "MyChessPiece.h"
#include "MyMarkers.h"

void printBoardScale(Mat& image);
void endGame(Mat& imageBackground, vector<MyChessPiece>& chessPieces);

int main() {
	MyMarkers markers = MyMarkers();
	Mat backgroundFrame; 
	int initImage = 1;

	vector<MyChessPiece> chessPieces;
	for (int i = 0; i < 32; i++) {
		chessPieces.push_back(MyChessPiece(i));
	}
	
	MyVideoCapture videoGet = MyVideoCapture(1);
	videoGet.beginTimer();
	while (!videoGet.stop) {	
		videoGet.getFrame();
		videoGet.updateFPS();

		detectMarkers(videoGet.frame, markers.markerDictionary, markers.corners, markers.ids);
		if (markers.ids.size() > 0) {
			markers.drawMyMarkers(videoGet.frame, chessPieces);
		}

		if (initImage) {
			videoGet.frame.copyTo(backgroundFrame);
			initImage = 0;
		}
		videoGet.printFPS(videoGet.frame);
		printBoardScale(videoGet.frame);
		imshow("Output", videoGet.frame);

		char key = (char)waitKey(30);
		if (key == 27) {
			videoGet.stop = true;
			videoGet.getFrame();
			endGame(backgroundFrame, chessPieces);
			break;
		}
	}
	return 0;
}

void printBoardScale(Mat& image) {
	Point2f printPoint1, printPoint2;
	char boardColumn, boardRow;
	string text;

	for (int i = 0; i < 8; i++) {
		if (i == 0) {
			printPoint1.x = 155;
			printPoint1.y = 475;
			printPoint2.y = 425;
			printPoint2.x = 110;

		}
		else {
			printPoint2.y -= 53;
			printPoint1.x += 53;

		}
		boardColumn = (char)(i + 65);
		text = boardColumn;
		putText(image, text, printPoint1, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 1, LINE_AA);
		boardRow = (char)(i + 49);
		text = boardRow;
		putText(image, text, printPoint2, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 1, LINE_AA);
	}
}

void endGame(Mat& imageBackground, vector<MyChessPiece>& chessPieces) {
	printBoardScale(imageBackground);
	for (int i = 0; i < 32; i++)
	{
		Mat imageChessBoard;
		imageBackground.copyTo(imageChessBoard);
		chessPieces[i].drawHeatMap(imageChessBoard);
	}
}



