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
#include <sys/stat.h>
#include <ctime>
using namespace std;
using namespace cv;

static int errorCount;
static long startTime;
static ofstream printer;
//takes a picture
//NOTE: picture currently saves in
Mat capture(VideoCapture cap) {
    Mat frame;
    cap.grab(); // grabs next frame
    cap.read(frame); // saves picture to frame

    // saves picture as .jpeg (includes compression)
    //bool success = imwrite("Pictures/" + name + ".jpeg", frame);
    //imwrite(name + ".png", frame);

    return frame;
}

bool compress(const Mat &frame, const String &name) {
    return imwrite("Pictures/" + name + ".jpeg", frame);
}

//calibrates camera appropriately
bool calibrate() {
    //TODO - build this
    //research indicates that cameras only need to be re-calibrated once a year.
    return false;
}

void logError(int level, const String &msg) {
    //gathering sys time
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    long ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    string time = to_string(ltm -> tm_hour) + ":" + to_string(1 + ltm -> tm_min) + ":" + to_string(1 + ltm -> tm_sec) + " (" + to_string(ms) + ")";


    if (!printer.is_open()) {
        cerr << "[" << time << "] " << "ErrorLogs Failed to open, no errors today!";
    }
    else {
        if (level == -1) { //new line
            printer << endl;
            return;
        }

        printer << "[" << time << "] ";
        switch(level) { //error tags
            case (1) :
                printer << "[SEVERE] "; //program breaking errors
                errorCount++;
                break;
            case (2) :
                printer << "[MODERATE] "; //things to look into
                errorCount++;
                break;
            case (99) :
                printer << "[ACTION] "; //logging important actions
                break;
            case (999) : //no tag
                break;
            default :
                printer << "[LEVEL " << level << "] "; //custom level errors
                errorCount++;
                break;
        }
        printer << msg << endl;
    }
    printer.flush();
}

int main(int argc, const char * argv[]) {
    startTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    errorCount = 0;
    int dirResult = mkdir("Error Logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //creating directory with basic read/write permissions
    if (dirResult != -1) {
        cerr << "Error Creating Error Logs Folder";
        errorCount++;
        //exit(1); //might be an idea to mitigate around this
    }
    dirResult = mkdir("Pictures", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //creating directory with basic read/write permissions
    if (dirResult != -1) {
        cerr << "Error Creating Pictures Folder";
        errorCount++;
        //exit(1); //might be an idea to mitigate around this
    }

    //gathering sys time
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    string year = to_string(1900 + ltm -> tm_year);
    string month = to_string(1 + ltm -> tm_mon);
    string day = to_string(ltm -> tm_mday);

    printer.open("Error Logs/" + year + "-" + month + "-" + day + ".txt", ios_base::app); //opening file or creating new one if does not exist


    logError(999, "START OF RUNTIME");
    // TODO - Create config.txt (or fancy, config.xml)
    // TODO - read from file for path name, camera ID and maybe other things if needed


    //--- INITIALIZE VIDEOCAPTURE

    // initializing using default API
    //logError(99, "Initializing cameras...");

    // camera 0
    VideoCapture capture0;
    capture0.open(0);

    // camera 1
    VideoCapture capture1;
    capture1.open(1);


    // OR advance usage: select any API backend
    //int deviceID = 0;             // 0 = open default camera
    //int apiID = CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    //cap.open(deviceID + apiID);


    //take picture and test for success
    //TODO - Figure out better naming system for two photos

    logError(99, "Taking pictures...");

    Mat picture0;
    Mat picture1;
    Mat *p0 = &picture0;
    Mat *p1 = &picture1;

    //TODO - Potentially implement multithreading here :)
    if (capture0.isOpened()) {
        picture0 = capture(capture0);
        logError(99, "Camera 0 frame grab");

    } else logError(2, "Camera 0 failed to open");

    if (capture1.isOpened()) {
        picture1 = capture(capture1);
        logError(99, "Camera 1 frame grab");

    } else logError(2, "Camera 1 failed to open");

    long endTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    logError(999, "Picture Taking Time: " + to_string(endTime - startTime) + "ms");

//    capture0.release();
//    logError(99, "Camera0 released");
//    capture1.release();
//    logError(99, "Camera1 released");

    logError(99, "Compressing...");

    //checking if pictures saved, if saved, compress
    //camera 0
    if (p0 != nullptr) {
        if (compress(picture0, "name" "(C0)"))
            logError(99, "Camera 0 Success! Saved at name (C0)");
        else
            logError(1, "Camera 0 failed but opened, possible defect or bug");
    }

    //camera 1
    if (p1 != nullptr) {
        if (compress(picture1, "name" "(C1)"))
            logError(99, "Camera 1 Success! Saved at name (C1)");
        else
            logError(1, "Camera 1 failed but opened, possible defect or bug");
    } 


    logError(999, "END OF RUNTIME");

    endTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    logError(999, "Runtime: " + to_string(endTime - startTime) + "ms - Error Count: " + to_string(errorCount));
    logError(-1, "");
    //destroying objects
    capture0.release();
    capture1.release();
    printer.close();
    return 0;
}


