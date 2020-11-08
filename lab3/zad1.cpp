#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace cv;

int main()
{
	int loRange[3] = {100, 45, 70};
	int hiRange[3] = {180, 225, 250};

	VideoCapture camera(0);

	while (waitKey(1) != 27)
	{
		Mat frame, frameMask, frameLimMask;

		camera >> frame;
		flip(frame, frame, 1);

		cvtColor(frame, frameMask, COLOR_BGR2HSV);
		inRange(frameMask, Scalar(loRange[0], loRange[1], loRange[2]), Scalar(hiRange[0], hiRange[1], hiRange[2]), frameLimMask);

		vector<vector<Point>> contours;
		findContours(frameLimMask, contours, RETR_LIST, CHAIN_APPROX_TC89_KCOS);

		if(contours.size() > 1)
        {
           Rect table[2] = {Rect(), Rect()};

           for(auto c:contours)
           {
               Rect r = boundingRect(c);
               int area = r.area();
               if(area > table[0].area())   //szukanie największych rectangle
                {
                   table[0] = r;
                } else if (area > table[1].area())
                {
                   table[1] = r;
                }
            }
            for(int i = 0; i < 2; i++)
            {
                rectangle(frame, table[i], {255, 0, 0}, 2);
            }
            if(table[0].y <= table[1].y + 60 && table[1].y <= table[0].y + 60) //różnica pixeli, porównanie z górnym lewym kątem 
            {
            line(frame, //rysowanie linii
            {
                table[0].x + table[0].width / 2, table[0].y + table[0].height / 2
            },
            {
                table[1].x + table[1].width / 2, table[1].y + table[1].height / 2
            },
            {0, 0, 255}, 4); //color, thickness
            }
        }
        imshow("contours", frame);
	}
	return 0;
}