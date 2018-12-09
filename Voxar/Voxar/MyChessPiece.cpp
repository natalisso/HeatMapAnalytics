#include "MyChessPiece.h"

MyChessPiece::MyChessPiece() {}

MyChessPiece::MyChessPiece(int PieceId) {
	id = PieceId;
	center.x = -1.0;
	center.y = -1.0;

	if (id >= 0 && id <= 15) {
		R = 51;
		G = 102;
		B = 255; 
	}
	else {
		R = 51;
		G = 204;
		B = 51;
	}

	if (id % 16 == 0 || id % 16 == 7)
		name = "Rook";
	else if (id % 16 == 1 || id % 16 == 6)
		name = "Knight";
	else if (id % 16 == 2 || id % 16 == 5)
		name = "Bishop";
	else if (id % 16 == 3)
		name = "Queen";
	else if (id % 16 == 4)
		name = "King";
	else
		name = "Pawn";
}

void MyChessPiece::findCenter(Point2f point1, Point2f point2) {

	if (point1.x < point2.x)
		center.x = point1.x + ((abs(point2.x - point1.x)) / 2);
	else
		center.x = point2.x + ((abs(point2.x - point1.x)) / 2);

	if (point1.y < point2.y)
		center.y = point1.y + ((abs(point2.y - point1.y)) / 2);
	else
		center.y = point2.y + ((abs(point2.y - point1.y)) / 2);
}

void MyChessPiece::storeTrackingPoints() {
	size_t vectorSize = trackingPoints.size();
	Point2f point;
	if (vectorSize == 0)
		trackingPoints.push_back(center);
	else {
		if (abs(trackingPoints[vectorSize - 1].x - center.x) > 20
			|| abs(trackingPoints[vectorSize - 1].y - center.y) > 20) {
			if (name == "Knight") {
				point.y = center.y;
				point.x = trackingPoints[vectorSize - 1].x;
				trackingPoints.push_back(point);
			}

			trackingPoints.push_back(center);
		}
	}
}

void MyChessPiece::drawHeatMap(Mat& imageChessBoard) {
	size_t vectorSize = trackingPoints.size();
	String imageName;
	ostringstream convert;

	if (vectorSize > 0) {
		for (int i = 1; i < vectorSize; i++) {
			line(imageChessBoard, trackingPoints[i - 1], trackingPoints[i], Scalar(B, G, R), 2, LINE_AA, 0);
			if (i == 1)
				circle(imageChessBoard, trackingPoints[i - 1], 2, Scalar(B, G, R), 2, LINE_AA, 0);
			if (i == vectorSize - 1) {
				circle(imageChessBoard, trackingPoints[i], 2, Scalar(B, G, R), 2, LINE_AA, 0);
				putText(imageChessBoard, "End", trackingPoints[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(51, 0, 153), 1, LINE_AA);
			}
		}
	}

	if (id >= 0 && id <= 15)
		imageName = "White_";
	else
		imageName = "Black_";

	convert << imageName << name << "_" << id << ".jpg";
	imwrite(convert.str(), imageChessBoard);
}
