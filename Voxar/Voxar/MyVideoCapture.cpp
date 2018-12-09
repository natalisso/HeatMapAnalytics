#include "MyVideoCapture.h"

MyVideoCapture::MyVideoCapture(int sourceOption) {
	startTime = 0;
	totalTime = 0;
	fps = 0;
	frameCounter = 0;
	source = sourceOption;
	stop = false;
	inputVideo.open(source);
}

void MyVideoCapture::getFrame() {
	if (!inputVideo.isOpened())
		stop = true;
	else 
		inputVideo.read(frame);

	if (stop)
		inputVideo.release();
}

void MyVideoCapture::beginTimer() {
	startTime = clock();
}

void MyVideoCapture::updateFPS() {
	frameCounter++;
	totalTime = (double)(clock() - startTime) / 1000;
	fps = frameCounter / totalTime;
}

void MyVideoCapture::printFPS(Mat& image) {
	// Trunca o valor em fps para mostrar no frame atual só com 2 casas decimais 
	double framesPerSecond = fps;
	framesPerSecond *= 100;
	framesPerSecond = trunc(framesPerSecond);
	framesPerSecond /= 100;

	ostringstream convert;
	convert << "FPS: " << framesPerSecond;
	putText(image, convert.str(), Point2f(15, 20), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 1, LINE_AA);
}


