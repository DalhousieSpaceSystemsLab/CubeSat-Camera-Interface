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


//takes a picture
//NOTE: picture currently saves in
bool capture(VideoCapture cap, const String &name) {
    Mat frame;
    cap.grab(); // grabs next frame
    cap.read(frame); // saves picture to frame

    // saves picture as .jpeg (includes compression)
    bool success = imwrite(name + ".jpeg", frame);
    //imwrite(name + ".png", frame);
    return success;
}

//calibrates camera appropriately
bool calibrate() {
    //TODO - build this
    return false;
}

int main(int argc, const char * argv[]) {
    // TODO - Create config.txt (or fancy, config.xml)
    // TODO - read from file for path name, camera ID and maybe other things if needed


    //--- INITIALIZE VIDEOCAPTURE
    //TODO - open two cameras
    //TODO - test if cameras exist
    VideoCapture cap;
    // open the default camera using default API
    cap.open(0);
    // for multiple cameras, different ID

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

    //take picture and test for success
    if (capture(cap, "name"))
        cout << "Picture Success!";
    else
        cout << "Picture failed :(";

    return 0;
}

