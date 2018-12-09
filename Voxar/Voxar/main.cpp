#include "MyVideoCapture.h"
#include "MyChessPiece.h"
#include "MyMarkers.h"

void printBoardScale(Mat& image);											// Imprime a identificação das linhas e colunas do tabuleiro
void endGame(Mat& imageBackground, vector<MyChessPiece>& chessPieces);		// Gera as imagens com o heat map de todas as peças

int main() {
	MyMarkers markers = MyMarkers();
	// Inicialização das peças 
	vector<MyChessPiece> chessPieces;
	for (int i = 0; i < 32; i++) {
		chessPieces.push_back(MyChessPiece(i));
	}
	
	// Inicializa a captura dos frames e o timer
	MyVideoCapture videoGet = MyVideoCapture(1);
	Mat initialFrame;
	int initFrame = 1;
	videoGet.beginTimer();

	while (!videoGet.stop) {	
		// Captura um novo frame e atualiza as variáveis para o fps
		videoGet.getFrame();
		videoGet.updateFPS();

		// Detecta e desenha os marcadores presentes no frame atual
		markers.detectMyMarkers(videoGet.frame, chessPieces);

		// Imprime a escala de linhas e colunas no frame atual
		printBoardScale(videoGet.frame);

		// Guarda o frame inicial com os desenhos dos marcadores
		if (initFrame) {
			videoGet.frame.copyTo(initialFrame);
			initFrame = 0;
		}

		// Imprime o fps no frame atual e mostrá-o na tela
		videoGet.printFPS(videoGet.frame);
		imshow("Output", videoGet.frame);

		// delay de 30 milissegundos e verifica se o ESC foi pressionado 
		char key = (char)waitKey(30);
		if (key == 27) {
			// Release na captura de tela e gera as imagens finais com os heat maps
			videoGet.stop = true;
			videoGet.getFrame();
			endGame(initialFrame, chessPieces);
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



