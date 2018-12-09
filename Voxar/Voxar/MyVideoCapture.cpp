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

void MyVideoCapture::getInitialFrame() {
	if (!inputVideo.isOpened())
		stop = true;
	else {
		inputVideo.read(initialFrame);
	}
}

void MyVideoCapture::getFrame() {
	if (!inputVideo.isOpened())
		stop = true;
	else 
		inputVideo.read(frame);

	if (stop)
		inputVideo.release();
}

void MyVideoCapture::showFrame() {
	imshow("Output", frame);

	char key = (char)waitKey(20);
	if (key == 27) {
		stop = true;
	}
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
	double framesPerSecond = fps;
	framesPerSecond *= 100;
	framesPerSecond = trunc(framesPerSecond);
	framesPerSecond /= 100;
	ostringstream convert;
	convert << "FPS: " << framesPerSecond;
	putText(image, convert.str(), Point2f(15, 20), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 1, LINE_AA);
}


