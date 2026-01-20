#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cout << "Error: Could not open webcam." << endl;
        return -1;
    }


    Scalar lowerColor(100, 150, 0); 
    Scalar upperColor(140, 255, 255);

    Mat frame, hsv, mask;

    while (true) {

cap >> frame;
        if (frame.empty()) break;

        flip(frame, frame, 1);

        cvtColor(frame, hsv, COLOR_BGR2HSV);

        inRange(hsv, lowerColor, upperColor, mask);

        erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        if (!contours.empty()) {
            double maxArea = 0;
            int maxIdx = -1;
            for (int i = 0; i < contours.size(); i++) {
                double area = contourArea(contours[i]);
                if (area > maxArea) {
                    maxArea = area;
                    maxIdx = i;
                }
            }

            if (maxIdx != -1 && maxArea > 500) { 

              Rect box = boundingRect(contours[maxIdx]);
                
                Scalar hudColor(0, 255, 0); 
                
                rectangle(frame, box, hudColor, 2);

                int cx = box.x + box.width / 2;
                int cy = box.y + box.height / 2;
                line(frame, Point(cx - 10, cy), Point(cx + 10, cy), hudColor, 2);
                line(frame, Point(cx, cy - 10), Point(cx, cy + 10), hudColor, 2);

                string coords = "X:" + to_string(cx) + " Y:" + to_string(cy);
                putText(frame, "TARGET LOCKED [ACTIVE]", Point(box.x, box.y - 25), FONT_HERSHEY_SIMPLEX, 0.6, hudColor, 2);
                putText(frame, coords, Point(box.x, box.y - 10), FONT_HERSHEY_PLAIN, 1, hudColor, 1);
            }
        } else {
            putText(frame, "SCANNING...", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }

        imshow("Defense Tech Tracker - C++", frame);
        
        
        if (waitKey(30) == 27) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
