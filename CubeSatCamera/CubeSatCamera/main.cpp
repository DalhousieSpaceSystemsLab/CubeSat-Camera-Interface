//
//  main.cpp
//  CubeSatCamera
//
//  Created by Liam Hebert on 2018-12-21.
//  Copyright © 2018 Dal Orbital Science & Space Lab. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
using namespace std;
using namespace cv;


bool capture(VideoCapture cap, const String &name) {
    Mat frame;
    cap.grab();
    cap.read(frame);
    bool success = imwrite(name + ".jpeg", frame);
    imwrite(name + ".png", frame);
    cout << "Print success " << success << "\n";
    return success;
}

int main(int argc, const char * argv[]) {
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    cap.open(0);
    // OR advance usage: select any API backend
    //int deviceID = 0;             // 0 = open default camera
    //int apiID = CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    //cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    capture(cap, "name");
    return 0;
}

