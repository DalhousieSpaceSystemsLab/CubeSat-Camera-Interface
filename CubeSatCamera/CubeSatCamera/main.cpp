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
static bool quiet;
static ofstream printer;

//gets today's date in YYYY-MM-DD format
string getDate() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    string year = to_string(1900 + ltm -> tm_year);
    string month = to_string(1 + ltm -> tm_mon);
    string day = to_string(ltm -> tm_mday);
    return year + "-" + month + "-" + day;
}

//gets local time in HH:MM:SS format
string getTime() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    long ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    return to_string(ltm -> tm_hour) + ":" + to_string(ltm -> tm_min) + ":" + to_string(ltm -> tm_sec) + " (" + to_string(ms) + ")";
}

//grabs next frame from camera
Mat capture(VideoCapture cap) {
    Mat frame;
    cap.grab(); // grabs next frame
    cap.read(frame); // saves picture to frame
    return frame;
}

//takes frame and saves it at path with specified compression format
bool compress(const Mat &frame, const string &path, const string &name, const string &compression) {
    return imwrite(path + name + "." + compression, frame);
}

//calibrates camera appropriately
bool calibrate() {
    //TODO - build this
    //research indicates that cameras only need to be re-calibrated once a year.
    return false;
}

//logs errors and actions into Error Logs/(date).txt
void log(int level, const string &msg) {
    if (!quiet) { //if quiet, no logs are created
        if (!printer.is_open()) {
            cerr << "[" << getTime() << "] " << "ErrorLog Failed to open, no errors today!" << endl;
        }

        else {
            if (level == -1) { //new line
                printer << endl;
                return;
            }

            printer << "[" << getTime() << "] ";
            switch (level) { //error tags
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
        printer.flush(); //prints message to file
    }
}

int main(int argc, const char * argv[]) {
    startTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    errorCount = 0;

    //TODO - check if the dir exists, might reduce time
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

    //parameters
    quiet = false;
    bool useCam0 = true;
    bool useCam1 = true;
    bool defaultPath = true;
    string filePath;
    bool defaultName = true;
    string fileName;
    bool defaultCompression = true;
    string compression;

    //gathering commandline args
    string arg;
    for (int i = 1; i < argc; i++) {
        arg = argv[i];
        if (arg == "-q") //no logs
            quiet = true;
        else if (arg == "-nc0") //ignore camera 0
            useCam0 = false;
        else if (arg == "-nc1") //ignore camera 1
            useCam1 = false;
        //TODO - clean this up
        else if (arg == "-p") { //custom path
            if (i + 1 < argc) { //room for filename
                arg = argv[i + 1];
                if (arg.at(0) != '-') { //valid format
                    filePath = arg;
                    defaultPath = false;
                    i++;
                }
                else {
                    arg = argv[i];
                    cerr << "invalid path format";
                }
            } else cerr << "missing path string";
        }
        else if (arg == "-n") { //custom filename
            if (i + 1 < argc) {
                arg = argv[i + 1];
                if (arg.at(0) != '-') {
                    fileName = arg;
                    defaultName = false;
                    i++;
                }
                else {
                    arg = argv[i];
                    cerr << "invalid file name format";
                }
            } else cerr << "missing file name string";
        }
        else if (arg == "-c") { //custom compression
            if (i + 1 < argc) {
                arg = argv[i + 1];
                if (arg.at(0) != '-') {
                    compression = arg;
                    defaultCompression = false;
                    i++;
                }
                else {
                    arg = argv[i];
                    cerr << "invalid compression format";
                }
            } else cerr << "missing compression string";
        }
    }

    //gathering sys date
    string date = getDate();
    if (!quiet) { //create logs if not in quiet mode
        printer.open("Error Logs/" + date + ".log", ios_base::app); //opening file or creating new one if does not exist
        log(999, "START OF RUNTIME"); //I know what you are thinking, I lied here. shhhh
    }
    if (defaultPath) //no custom path
        filePath = "Pictures/";
    if (defaultName) //no custom file name
        fileName = date;
    if (defaultCompression) //no custom compression method
        compression = "jpeg";

    // initializing using default API







    //take picture and test for success

    log(99, "Taking pictures...");




    //TODO - Potentially implement multithreading here :)
    // camera 0

    VideoCapture capture0;
    if (useCam0) {
        log(99, "Using Camera 0...");
        capture0.open(0);
    }
    Mat picture0;
    if (capture0.isOpened()) {
        picture0 = capture(capture0);
        log(99, "Camera 0 frame grab");
    }
    else if (useCam0)
        log(2, "Camera 0 failed to open");


    // camera 1
    VideoCapture capture1;
    if (useCam1) {
        log(99, "Using Camera 1...");
        capture1.open(1);
    }

    Mat picture1;
    if (capture1.isOpened()) {
        picture1 = capture(capture1);
        log(99, "Camera 1 frame grab");

    }
    else if (useCam1)
        log(2, "Camera 1 failed to open");

    long endTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    log(999, "Picture Taking Time: " + to_string(endTime - startTime) + "ms");


    log(99, "Compressing...");

    //checking if pictures saved, if saved, compress
    //camera 0
    if (capture0.isOpened()) {
        if (compress(picture0, filePath, fileName + " (C0)", compression))
            log(99, "Camera 0 Success! Saved at " + filePath + fileName + " (C1)" + "." + compression);
        else
            log(1, "Camera 0 failed but opened, possible defect or bug");
    }

    //camera 1
    if (capture1.isOpened()) {
        if (compress(picture1, filePath, fileName + " (C1)", compression))
            log(99, "Camera 1 Success! Saved at " + filePath + fileName + " (C1)" + "." + compression);
        else
            log(1, "Camera 1 failed but opened, possible defect or bug");
    }


    log(999, "END OF RUNTIME");

    endTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    long runTime = endTime - startTime;
    if (quiet)
        cout << "Runtime: " << to_string(runTime) << "ms";
    else {
        log(999, "Runtime: " + to_string(runTime) + "ms - Error Count: " + to_string(errorCount));
        log(-1, "");
    }
    //destroying
    capture0.release();
    capture1.release();
    printer.close();
    return 0;
}


