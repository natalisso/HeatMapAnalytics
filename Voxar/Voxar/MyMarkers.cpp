#include "MyMarkers.h"

MyMarkers::MyMarkers() {
	markerDictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);
}

void MyMarkers::generateMarkers() {
	Mat outputMarker;
	string imageName = "5x5Marker_";

	for (int i = 0; i < 32; i++)
	{
		drawMarker(markerDictionary, i, 140, outputMarker, 1);  // Gera os marcadores conforme presente no markerDictionary com tamanho 140x140 pixels

		// Salva a imagem do marcado em uma imagem .jpg
		ostringstream convert;
		convert << imageName << i << ".jpg";
		imwrite(convert.str(), outputMarker);
	}
}

void MyMarkers::detectMyMarkers(Mat& currentFrame,  vector<MyChessPiece>& allChessPieces) {
	Point2f newCenter, printCenter;

	detectMarkers(currentFrame, markerDictionary, corners, ids);  // Detecta todos os marcadores presentes em markerDictionary que estão no frame atual 
																  //e guarda as posições dos 4 cantos em corners e o id em ids
	
	if (ids.size() > 0) {
		for (int i = 0; i < corners.size(); i++) {
			ostringstream convert;
			convert << allChessPieces[ids[i]].id;

			// Desenha um retângulo na borda do marcador com a cor que representa a cor da peça (azul -> brancos; verde -> pretos)
			rectangle(currentFrame, corners[i][0], corners[i][2], Scalar(allChessPieces[ids[i]].B, allChessPieces[ids[i]].G, allChessPieces[ids[i]].R), 2, LINE_AA);

			// Printa o nome da peça na frente do marcador
			if (allChessPieces[ids[i]].id >= 0 && allChessPieces[ids[i]].id <= 15)
				printCenter = corners[i][0];
			else {
				printCenter = corners[i][3];
				printCenter.y += 10;
			}
			putText(currentFrame, allChessPieces[ids[i]].name, printCenter, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(51, 0, 153), 1, LINE_AA);

			// Printa o id do marcador no meio dele
			printCenter.y = corners[i][0].y + (abs(corners[i][3].y - corners[i][0].y))*0.7;
			printCenter.x = corners[i][0].x + (abs(corners[i][1].x - corners[i][0].x)) / 4;
			putText(currentFrame, convert.str(), printCenter, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(68, 0, 204), 2, LINE_AA); 

			// Calcula o novo centro da peça e guarda no vetor de tracking points da peça
			allChessPieces[ids[i]].findCenter(corners[i][0], corners[i][2]);
			allChessPieces[ids[i]].storeTrackingPoints();			// Atualiza a posição do marcador no vetor do tracking 
		}
	}
}
