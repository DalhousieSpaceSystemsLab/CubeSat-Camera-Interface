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

//gets today's date in YYYY-MM-DD format
string CubeSatCamera::getDate() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  string year = to_string(1900 + ltm -> tm_year);
  string month = to_string(1 + ltm -> tm_mon);
  string day = to_string(ltm -> tm_mday);
  return year + "-" + month + "-" + day;
}

cameraParams_t CubeSatCamera::parseParams(vector<std::string> argv) {
  //gathering commandline args
  string arg;

  //default params
  cameraParams_t param;
  param.compression = "jpeg";
  param.date = getDate();
  param.fileName = param.date;
  param.filePath = "Pictures/";
  param.quality = 0;
  

  for (int i = 0; i < argv.size(); i++) { //sadly switch statements cannot be used here without hashing, which has the potential to be inaccurate and is not worth the time loss
    arg = argv.at(i);
    if (arg == "-p") { //custom path
      if (i + 1 < argv.size()) { //room for filename
        arg = argv.at(i+1);
        if (arg.at(0) != '-') { //valid format
          param.filePath = arg;
        }
        else {
          arg = argv.at(1);
          cerr << "invalid path format" << endl;
        }
      } else cerr << "missing path string" << endl;
    }
    else if (arg == "-n") { //custom filename
      if (i + 1 < argv.size()) {
        arg = argv.at(i + 1);
        if (arg.at(0) != '-') {
          param.fileName = arg;
          i++;
        }
        else {
          arg = argv.at(i);
          cerr << "invalid file name format" << endl;
        }
      } else cerr << "missing file name string" << endl;
    }
    else if (arg == "-c") { //custom compression
      if (i + 1 < argv.size()) {
        arg = argv.at(i + 1);
        if (arg.at(0) != '-') {
          param.compression = arg;
          if (arg == "jpeg" && param.quality == 0) 
            param.quality = 90;
          else if (arg == "png" && param.quality == 0)
            param.quality = 8;
          i++;
        }
        else {
          arg = argv.at(i + 1);
          cerr << "invalid compression format" << endl;
        }
      } else cerr << "missing compression string" << endl;
    }
    else if (arg == "-iq") {
      if (i + 1 < argv.size()) {
        arg = argv.at(i + 1);
        if (arg.at(0) != '-') {
          param.quality = stoi(arg); //will check what the real format is later
          i++;
        }
        else {
          arg = argv.at(i);
          cerr << "invalid compression format" << endl;
        }
      } else cerr << "missing compression string" << endl;
    }
  }
  return param;
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
bool CubeSatCamera::compress(const Mat &frame, const std::string &path, const std::string &name, const std::string &compression, const int quality) {
  vector<int> compression_params;
  if (compression == "jpeg") {
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(quality);
    printf("JPEG");
  }
  else if (compression == "png") {
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(quality);
    printf("PNG");
  }
  printf("%lu", compression_params.size());
  printf("\n%s", compression.c_str());
  return imwrite(path + name + ".jpeg", frame);
}

bool CubeSatCamera::grab(int camera, cameraParams_t param) {
  return grab(camera, param.filePath, param.fileName, param.compression, param.quality);
}

bool CubeSatCamera::grab(int camera, const std::string &filePath, const std::string &fileName, const std::string &compression, int quality) {
  //talk about settings used
  log(NONE, "--------------------------------");
  log(ACTION, "SETTINGS");
  log(NONE, "Date: " + getDate());
  log(NONE, "File Path: " + filePath);
  log(NONE, "File Name: " + fileName);
  log(NONE, "Compression: " + compression);
  log(NONE, "Quality Setting: " + to_string(quality));
  log(NONE, "--------------------------------");
  log(NONE, "");

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
      if(compress(picture, filePath, fileName + " (C" + to_string(camera) + ")", compression, quality)){
        log(ACTION, "Compressed successfully! Saved at " + filePath + fileName + " (C" + to_string(camera) + ")" + "." + compression);
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


bool CubeSatCamera::release() {
  C0.release();
  C1.release();
  printer.close();
  return true;
}

bool CubeSatCamera::init() {
  int dirResult = mkdir("Error Logs", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //creating dir w/ rwx-rx-rx
  if (dirResult != -1) { //-1 means folder already exists
    cout << "Creating new Error Logs Folder..." << endl;
  }
  dirResult = mkdir("Pictures", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //creating dir w/ rwx-rx-rx
  if (dirResult != -1) {
    cout << "Creating new Pictures Folder..." << endl;
  }

  printer.open("Error Logs/" + getDate() + ".log", ios_base::app); //opening file or creating new one if does not exist
  log(NONE, "START OF RUNTIME"); //I know what you are thinking, I lied here. shhhh

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
