//
//  CubeSatCamera.cpp
//  CubeSatCamera
//
//  Created by Liam Hebert on 2018-12-21.
//  Copyright © 2018 Dal Orbital Science & Space Lab. All rights reserved.
//

#include "CubeSatCamera.hpp"

// struct parameters_t {
//     string date;
//     bool quiet;
//     string filePath;
//     string fileName;
//     string compression;
//     int quality;
// };

// class CubeSatCamera {
// public:
//     bool init( int argv.size(), const char * argv[] );
//     bool grab( int camera );
//     bool release();
//     void log ( const int level, const string &msg );
//
//     string getFileName();
//     CubeSatCamera( int argv.size(), const char * argv[] ) {
//         init(argv.size(), argv);
//     }
//     ~CubeSatCamera() {
//         release();
//     }
// private:
//     int errorCount;
//     ofstream printer;
//
//     VideoCapture C0;
//     VideoCapture C1;
//
//     parameters_t * param;
//     string getDate();
//     string getTime();
//     Mat capture( VideoCapture *cap );
//     bool compress ( const Mat &frame, const string &path, const string &name, const string &compression );
// };

string CubeSatCamera::getFileName() {
  return param.fileName;
}

//gets today's date in YYYY-MM-DD format
string CubeSatCamera::getDate() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  string year = to_string(1900 + ltm -> tm_year);
  string month = to_string(1 + ltm -> tm_mon);
  string day = to_string(ltm -> tm_mday);
  return year + "-" + month + "-" + day;
}

//gets local time in HH:MM:SS format
string CubeSatCamera::getTime() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  long ms = CURRENT_TIME;
  return to_string(ltm -> tm_hour) + ":" + to_string(ltm -> tm_min) + ":" + to_string(ltm -> tm_sec) + " (" + to_string(ms) + ")";
}

//grabs next frame from camera
Mat CubeSatCamera::capture(VideoCapture *cap) {
  Mat frame;
  (*cap).read(frame); // saves picture to frame
  return frame;
}

//takes frame and saves it at path with specified compression format
bool CubeSatCamera::compress(const Mat &frame, const string &path, const string &name, const string &compression) {
  return imwrite(path + name + "." + compression, frame);
}

bool CubeSatCamera::grab(int camera) {
  long startTime = CURRENT_TIME;
  VideoCapture * cam;
  Mat picture;
  if (camera == 0)
  cam = &C0;
  else if (camera == 1)
  cam = &C1;
  else {
    picture.release();
    log(MODERATE, "Invalid camera selection");
    return false;
  }
  log(NONE, "--------------------------------");
  log(ACTION, "Using Camera " + to_string(camera) + "...");
  if (cam -> isOpened()) {
    picture = capture(cam);
    if (!picture.empty()) {
      log(ACTION, "Frame grabbed successfully");
      if(compress(picture, param.filePath, param.fileName + " (C" + to_string(camera) + ")", param.compression)){
        log(ACTION, "Compressed successfully! Saved at " + param.filePath + param.fileName + " (C" + to_string(camera) + ")" + "." + param.compression);
        picture.release();
        log(ACTION, "Runtime: " + to_string(CURRENT_TIME - startTime));
        log(NONE, "--------------------------------");
        log(NONE, "");
        return true;
      }
      else { //compression failed
        log(SEVERE, "Picture failed to compress");
        picture.release();
      }
    }
    else { //frame grab failed
      log(SEVERE, "Camera failed to grab frame");
      picture.release();
    }
  }
  else { //camera not properly initialized
    log(MODERATE, "Camera not properly initialized");
    picture.release();
  }
  log(ACTION, "Runtime: " + to_string(CURRENT_TIME - startTime));
  log(NONE, "--------------------------------");
  log(NONE, "");
  return false;
}

//logs errors and actions into Error Logs/(date).txt
void CubeSatCamera::log(const int level, const string &msg) {
  if (!(param.quiet)) { //if quiet, no logs are created
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
        case (SEVERE) :
        printer << "[SEVERE] "; //program breaking errors
        errorCount++;
        break;
        case (MODERATE) :
        printer << "[MODERATE] "; //things to look into
        errorCount++;
        break;
        case (ACTION) :
        printer << "[ACTION] "; //logging important actions
        break;
        case (SYSTEM) :
        printer << "[SYSTEM] "; //logging external input
        break;
        case (NONE) : //no tag
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

bool CubeSatCamera::release() {
  C0.release();
  C1.release();
  printer.close();
  return true;
}

bool CubeSatCamera::init(string argv[], int argc) {
  errorCount = 0;
  int dirResult = mkdir("Error Logs", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //creating dir w/ rwx-rx-rx
  if (dirResult != -1) { //-1 means folder already exists
    cout << "Creating new Error Logs Folder..." << endl;
    errorCount++;
  }
  dirResult = mkdir("Pictures", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //creating dir w/ rwx-rx-rx
  if (dirResult != -1) {
    cout << "Creating new Pictures Folder..." << endl;
  }

  param.date = getDate();
  param.quiet = false;
  param.filePath = "Pictures/";
  param.fileName = param.date;
  param.compression = "jpeg";
  param.quality = 95;

  //TODO - Refactor this badboy into the grab commands. No reason for this to be here anymore.
  //gathering commandline args
  string arg;
  for (int i = 1; i < argc; i++) { //sadly switch statements cannot be used here without hashing, which has the potential to be inaccurate and is not worth the time loss
    arg = argv[i];
    if (arg == "-q") //no logs
    param.quiet = true;
    else if (arg == "-p") { //custom path
      if (i + 1 <= argc) { //room for filename
        arg = argv[i + 1];
        if (arg.at(0) != '-') { //valid format
          param.filePath = arg;
        }
        else {
          arg = argv[i];
          cerr << "invalid path format" << endl;
        }
      } else cerr << "missing path string" << endl;
    }
    else if (arg == "-n") { //custom filename
      if (i + 1 <= argc) {
        arg = argv[i + 1];
        if (arg.at(0) != '-') {
          param.fileName = arg;
          i++;
        }
        else {
          arg = argv[i];
          cerr << "invalid file name format" << endl;
        }
      } else cerr << "missing file name string" << endl;
    }
    else if (arg == "-c") { //custom compression
      if (i + 1 <= argc) {
        arg = argv[i + 1];
        if (arg.at(0) != '-') {
          param.compression = arg;
          i++;
        }
        else {
          arg = argv[i];
          cerr << "invalid compression format" << endl;
        }
      } else cerr << "missing compression string" << endl;
    }
    //TODO think about implementing this
    else if (arg == "-iq") {
      if (i + 1 <= argc) {
        arg = argv[i + 1];
        if (arg.at(0) != '-') {
          param.quality = stoi(arg); //will check what the real format is later
          i++;
        }
        else {
          arg = argv[i];
          cerr << "invalid compression format" << endl;
        }
      } else cerr << "missing compression string" << endl;
    }
  }
  //argv.clear();


  if (!(param.quiet)) { //create logs if not in quiet mode
    printer.open("Error Logs/" + param.date + ".log", ios_base::app); //opening file or creating new one if does not exist
    log(NONE, "START OF RUNTIME"); //I know what you are thinking, I lied here. shhhh

    //talk about settings used
    log(NONE, "--------------------------------");
    log(ACTION, "SETTINGS");
    log(NONE, "Date: " + param.date);
    log(NONE, "Quiet Mode: " + to_string(param.quiet));
    log(NONE, "File Path: " + param.filePath);
    log(NONE, "File Name: " + param.fileName);
    log(NONE, "Compression: " + param.compression);
    log(NONE, "Quality Setting: " + to_string(param.quality));
    log(NONE, "--------------------------------");
    log(NONE, "");
  }

  //initializing cameras here
  log(NONE, "--------------------------------");
  log(ACTION, "Initializing camera 0...");
  C0.open(0);


  if (C0.isOpened())
    log(ACTION, "Initialized");
  else
    log(SEVERE, "Camera 0 failed to initialize");
    
  log(ACTION, "Initializing camera 1...");
  C1.open(1);
  if (C1.isOpened())
    log(ACTION, "Initialized");
  else
    log(SEVERE, "Camera 1 failed to initialize");
  return 0;
}
