//
//  CubeSatCamera.cpp
//  CubeSatCamera
//
//  Created by Liam Hebert on 2018-12-21.
//  Copyright © 2018 Dal Orbital Science & Space Lab. All rights reserved.
//

#include "CubeSatCamera.hpp"

//gets today's date in YYYY-MM-DD format
string CubeSatCamera::getDate() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  string year = to_string(1900 + ltm -> tm_year);
  string month = to_string(1 + ltm -> tm_mon);
  string day = to_string(ltm -> tm_mday);
  return year + "-" + month + "-" + day;
}

bool CubeSatCamera::isReady() {
  return C0.isOpened() || C1.isOpened();
}

/**
 * DEPRACATED: Users should use getParams instead
 */
void CubeSatCamera::parseParams(vector<std::string> argv, cameraParams_t * param) {
  //gathering commandline args
  string arg;

  //default params
  param -> compression = "jpeg";
  param -> date = getDate();
  param -> fileName = param -> date;
  param -> filePath = "Pictures/";
  param -> quality = 70;
  

  for (int i = 0; i < argv.size(); i++) { //sadly switch statements cannot be used here without hashing, which has the potential to be inaccurate and is not worth the time loss
    arg = argv.at(i);
    if (arg == "-p") { //custom path
      if (i + 1 < argv.size()) { //room for filename
        arg = argv.at(i+1);
        if (arg.at(0) != '-') { //valid format
          param -> filePath = arg;
        }
        else {
          cerr << "invalid path format" << endl;
        }
      } else cerr << "missing path string" << endl;
      continue;
    }
    else if (arg == "-n") { //custom filename
      if (i + 1 < argv.size()) {
        arg = argv.at(i + 1);
        if (arg.at(0) != '-') {
          param -> fileName = arg;
          i++;
        }
        else 
          cerr << "invalid file name format" << endl;
      } else cerr << "missing file name string" << endl;
      continue;
    }
    else if (arg == "-c") { //custom compression
      if (i + 1 < argv.size()) {
        arg = argv.at(i + 1);
        if (arg.at(0) != '-') {
          param -> compression = arg;
          if (arg == "jpeg" && param -> quality == 0) 
            param -> quality = 90;
          else if (arg == "png" && param -> quality == 0)
            param -> quality = 8;
          i++;
        }
        else 
          cerr << "invalid compression format" << endl;
      } else cerr << "missing compression string" << endl;
      continue;
    }
    else if (arg == "-iq") {
      if (i + 1 < argv.size()) {
        arg = argv.at(i + 1);
        if (arg.at(0) != '-') {
          param -> quality = stoi(arg); //will check what the real format is later
          i++;
        }
        else 
          cerr << "invalid quality format" << endl;
      } else cerr << "missing compression string" << endl;
      continue;
    }
  }
}

//gets local time in HH:MM:SS format
string CubeSatCamera::getTime() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  long ms = CURRENT_TIME;
  return to_string(ltm -> tm_hour) + ":" + to_string(ltm -> tm_min) + ":" + to_string(ltm -> tm_sec) + " (" + to_string(ms) + ")";
}

//takes frame and saves it at path with specified compression format
bool CubeSatCamera::compress(const Mat &frame, const cameraParams_t *params, string camera) {
  // vector<int> compression_params;
  // if (params -> compression == "jpeg") {
  //   compression_params.push_back(IMWRITE_JPEG_QUALITY);
  //   compression_params.push_back(params -> quality);
  // }
  // else if (params -> compression == "png") {
  //   compression_params.push_back(IMWRITE_PNG_COMPRESSION);
  //   compression_params.push_back(params -> quality);
  // }
  return imwrite(params -> filePath + params -> fileName + "(" + camera + ")" + "." + params -> compression, frame);
}

bool CubeSatCamera::grab(cameraParams_t * param) {
  bool result = false;
  //if either is successful, return true;
  if (C0.isOpened()) 
    result = result || grab(0, param);
  if (C1.isOpened()) {
    cout << "WHY" << endl;
    result = result || grab(1, param);;
  }
  return result;
}


bool CubeSatCamera::grab(int camera, cameraParams_t * param) {
  //talk about settings used
  long startTime = CURRENT_TIME;
  VideoCapture * cam = NULL;
  Mat picture;

  logger -> info("Using Camera " + to_string(camera) + "...");
  if (camera == 0)
    cam = &C0;
  else if (camera == 1)
    cam = &C1;
  else {
    logger -> warn("Invalid camera selection");
    return false;
  }
  
  *cam >> picture;
  if (!picture.empty()) {
    logger -> info("Frame grabbed successfully");
    if(compress(picture, param, "C" + to_string(camera))){
      logger -> info("Compressed successfully! Saved at " + param -> filePath + param -> fileName + " (C" + to_string(camera) + ")" + "." + param -> compression);
      logger -> info("Runtime: " + to_string(CURRENT_TIME - startTime));
      return true;
    }
    else  //compression failed
      logger -> critical("Picture failed to compress");
  }
  else //camera not properly initialized
    logger -> warn("Unable to take picture");
  logger -> info("Runtime: " + to_string(CURRENT_TIME - startTime));
  return false;
}


bool CubeSatCamera::release() {
  logger -> info("RELEASING CAMERAS");
  C0.release();
  C1.release();
  logger -> flush();
  return true;
}

void CubeSatCamera::flush() {
  logger -> flush();
}

bool CubeSatCamera::init() {
  logger -> info("INITIALIZING CAMERAS");

  int results = 0;
  //initializing cameras here
  logger -> info("Initializing camera 0...");
  C0.open(0);
  if (C0.isOpened()) {
    logger -> info("Initialized");
  }
  else {
    logger -> critical("Camera 0 failed to initialize");
  }
  // c1Open = false; 
  // logger -> info("Initializing camera 1...");

  // if (C1.open(1)) {
  //   c1Open = true;
  //   logger -> info("Initialized");
  // }
  // else {
  //   c1Open = false;
  //   logger -> critical("Camera 1 failed to initialize");
  // }
  return C0.isOpened() || C1.isOpened();
}
