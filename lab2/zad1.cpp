#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {

    VideoCapture cap(0);
    int width = 320;
    int height = 200;

    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }

    if ( !cap.isOpened() ) {
        cerr << "error opening frames source" << endl;
        return -1;
    }
    cout << "Video size: " << cap.get( CAP_PROP_FRAME_WIDTH ) 
    << "x" << cap.get( CAP_PROP_FRAME_HEIGHT ) << endl;

    namedWindow("hsv", WINDOW_AUTOSIZE);
    
    const int max_value_H = 360/2;
    const int max_value = 255;
    int low_H = 0, low_S = 0, low_V = 0;
    int high_H = max_value_H, high_S = max_value, high_V = max_value;

    
    createTrackbar("Low H", "hsv", &low_H, 255);
    createTrackbar("High H", "hsv", &high_H, 255);
    createTrackbar("Low S", "hsv", &low_S, 255);
    createTrackbar("High S", "hsv", &high_S, 255);
    createTrackbar("Low V", "hsv", &low_V, 255);
    createTrackbar("High V", "hsv", &high_V, 255);
    

    Mat frame, hsv;
    if ( cap.read( frame ) ) {

        resize(frame, frame, Size(width, height));
        GaussianBlur(frame, frame, Size(7,7), 0);
        //putText(hsv, to_string(range), {10, 30}, FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        
        do {
            cvtColor(frame, hsv, COLOR_BGR2HSV);
            inRange(hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), hsv);
            imshow( "Gaussian Blur", frame );
            imshow( "hsv", hsv );

            char k = waitKey(5);
            if (k == 27)
                break;

            if (k == 'x') {
                auto r = selectROI("hsv", hsv);
                Mat roi = frame(r);
                imshow("ROI", roi);
                cv::imwrite("roi.bmp", roi);
            }

            } while (true);
    }
    
    return 0;
}