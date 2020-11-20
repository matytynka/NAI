#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

void fixPointsOrder(vector<Point> in, vector<Point> &out) {
	sort(
		in.begin(),
		in.end(),
		[](auto &a, auto&b) {
			return a.y < b.y;
		}
	);
	sort(
		in.begin(),
		in.begin()+2,
		[](auto &a, auto&b) {
			return a.x < b.x;
		}
	);
	Point tl(in.at(0));
	Point tr(in.at(1));
	sort(
		in.end()-2,
		in.end(),
		[](auto &a, auto &b) {
			return a.x > b.x;
		}
	);
	Point br(in.at(2));
	Point bl(in.at(3));
	out.push_back(tl);
	out.push_back(tr);
	out.push_back(br);
	out.push_back(bl);
} 

void findAndCutDocument(Mat in, Mat &out) {
	Mat gray;

	cvtColor(in, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(5,5), 0);
    Canny(gray, gray, 75, 200);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(gray, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
	sort(
		contours.begin(),
		contours.end(),
		[](auto &a, auto &b) {
			return contourArea(a, false) > contourArea(b, false);
	 	}
    );

    vector<vector<Point>> approx(contours.size());
	size_t i, j;

	for(i = 0; i < contours.size(); i++) {
		double peri = arcLength(contours.at(i), true);
		approxPolyDP(contours.at(i), approx.at(i), (0.01 * peri), true);
	}
	
	vector<vector<Point>> docContour;
	for(i = 0; i < approx.size(); i++) {
		drawContours(in, approx, i, Scalar(0,225,225), 3);
		if(approx.at(i).size() == 4) {		
			docContour.push_back(approx.at(i));
		}
	}
	
	if (docContour.size() > 0) {

		vector<Point> fixedPoints;
		fixPointsOrder(docContour.at(0), fixedPoints);

		Mat dst(Size(480, 720), CV_8UC3);
		vector<Point2f> _src = {{0,0}, {(float)dst.cols, 0}, {(float)dst.cols, (float)dst.rows}, {0, (float)dst.rows}};
		vector<Point2f> _dst;
		
		for (auto p: fixedPoints) {
			_dst.push_back(Point2f(p.x, p.y));
		}

		auto wrapMat = getPerspectiveTransform(_dst, _src);
		warpPerspective(in, out, wrapMat, Size(dst.cols, dst.rows));
	}
}

void rotateDocument(Mat in, Mat &out) {        
	int scanHalfX = in.cols / 2;
	int scanHalfY = in.rows / 2;

	int cx, cy;

	//int loMarker[3] = {0, 185, 75};
	int loMarker[3] = {82, 87, 138};
	int hiMarker[3] = {255, 255, 255};

	Mat hsv;

	vector<vector<Point>> contours;

	cvtColor(in, hsv, COLOR_BGR2HSV);
	inRange(
		hsv,
		Scalar(loMarker[0], loMarker[1], loMarker[2]),
		Scalar(hiMarker[0], hiMarker[1], hiMarker[2]),
		hsv
	);

	findContours(hsv, contours, RETR_LIST, CHAIN_APPROX_TC89_KCOS);
	sort(
		contours.begin(),
		contours.end(),
		[](auto &a, auto &b) {
			return contourArea(a, false) > contourArea(b, false);
		}
	);

	if(contours.size() > 1) {
		Rect rect = Rect();
		for(auto c:contours) {
			Rect br = boundingRect(c);
			int area = br.area();
			if(area > rect.area()) {
				rect = br;
			}
		}
		rectangle(in, rect, Scalar(255,255,0), 3);
		cx = rect.x + rect.width / 2;
		cy = rect.y + rect.height / 2;


		if(cx < scanHalfX && cy < scanHalfY) {//Top left
		} else
		if (cx > scanHalfX && cy < scanHalfY) {//Top right
			rotate(in, out, ROTATE_90_COUNTERCLOCKWISE);
			resize(out, out, Size(in.cols, in.rows));
		} else
		if (cx < scanHalfX && cy > scanHalfY) {//Bottom left
			rotate(in, out, ROTATE_90_COUNTERCLOCKWISE);
			resize(out, out, Size(in.cols, in.rows));
		} else
		if (cx > scanHalfX && cy > scanHalfY) {//Bottom right
		rotate(in, out, ROTATE_180);
		}
	}
}

int main() {
    char key;
	VideoCapture cap(0);
	Mat frame, scan;

	do {
		if(cap.read(frame)) {
            key = (waitKey(1000.0/60.0)&0x0ff);

			findAndCutDocument(frame, scan);
			rotateDocument(scan, scan);
			imwrite("result.jpg", scan);
			resize(frame, frame, Size(720, 480));
            imshow("Scan", scan);
            imshow("Edge", frame);

		    if (key == 27) break;
        }
	} while (true);

	return 0;
}