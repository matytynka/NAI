#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <ctime>
#include <unistd.h>

using namespace std;
using namespace cv;

int carCount = 0;

void detect(const Mat& frame, CascadeClassifier& cascade) {
	Mat frameGray;
	vector<Rect> detectedCar;
	cvtColor(frame, frameGray, COLOR_BGR2GRAY);
	equalizeHist(frameGray, frameGray);
	cascade.detectMultiScale(frameGray, detectedCar, 1.1, 3, 0, Size(50,50));
	for (auto& det : detectedCar) {
		if(det.y > 496 && det.y < 504) {
			carCount++;
		}
		rectangle(frame, det, Scalar(255, 255, 0), 2);
	}
	String text = "Count: " + to_string(carCount);
	putText(frame,text,Point(10,480),FONT_HERSHEY_COMPLEX,2,Scalar(255,255,255),2);
	line(frame,Point(0,500),Point(1240,500),Scalar(255,255,255),2);
	
	imshow("Display", frame);
}

int main() {
	CascadeClassifier cascade;
	VideoCapture capture;
	Mat frame, small;

	capture.open("video.mkv");
	
	if (!cascade.load("cars.xml") || !capture.isOpened()) return -1;

	while (capture.read(frame)) {
		if (frame.empty()) {
			return -1;
		}
		resize(frame, small, Size(1240, 720));
		detect(small, cascade);
		if (waitKey(10) == 27)
			break;
	}
	return 0;
}