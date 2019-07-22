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
  return c0Open || c1Open;
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
          cerr << "invalid quality format" << endl;
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
  }
  else if (compression == "png") {
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(quality);
  }
  return imwrite(path + name + "." + compression, frame);
}

bool CubeSatCamera::grab(cameraParams_t param) {
  bool result = false;
  spdlog::info("VALUE OF c0Open {} c1Open {}", c0Open, c1Open);
  //if either is successful, return true;
  if (c0Open == true) {
    bool temp = grab(0, param.filePath, param.fileName, param.compression, param.quality);
    spdlog::info(temp);
    result = result || temp;
  }
  if (c1Open == true) {
    bool temp = grab(0, param.filePath, param.fileName, param.compression, param.quality);
    spdlog::info(temp);
    result = result || temp;
  }

  spdlog::info("VALUE OF RESULT {}" , result);
  return result;
}

bool CubeSatCamera::grab(int camera, const std::string &filePath, const std::string &fileName, const std::string &compression, int quality) {
  //talk about settings used
  stringstream stream;
  stream << endl << "--------------------------------" << endl;
  stream << "SETTINGS" << endl;
  stream << "Date: " << getDate() << endl;
  stream << "File Path: " << filePath << endl;
  stream << "File Name: " << fileName << endl;
  stream << "Compression: " + compression << endl;
  stream << "Quality Setting: " + to_string(quality) << endl;
  stream << "--------------------------------" << endl;
  logger -> info(stream.str());
  stream.clear();

  long startTime = CURRENT_TIME;
  VideoCapture * cam;
  Mat picture;
  if (camera == 0)
    cam = &C0;
  else if (camera == 1)
    cam = &C1;
  else {
    picture.release();
    logger -> warn("Invalid camera selection");
    return false;
  }
  logger -> info("Using Camera " + to_string(camera) + "...");
  if (cam -> isOpened()) {
    picture = capture(cam);
    if (!picture.empty()) {
      logger -> info("Frame grabbed successfully");
      if(compress(picture, filePath, fileName + " (C" + to_string(camera) + ")", compression, quality)){
        logger -> info("Compressed successfully! Saved at " + filePath + fileName + " (C" + to_string(camera) + ")" + "." + compression);
        picture.release();
        logger -> info("Runtime: " + to_string(CURRENT_TIME - startTime));
        return true;
      }
      else { //compression failed
        logger -> critical("Picture failed to compress");
        picture.release();
      }
    }
    else { //frame grab failed
      logger -> critical("Camera failed to grab frame");
      picture.release();
    }
  }
  else { //camera not properly initialized
    logger -> warn("Camera not properly initialized");
    picture.release();
  }
  logger -> info("Runtime: " + to_string(CURRENT_TIME - startTime));
  return false;
}


bool CubeSatCamera::release() {
  logger -> info("RELEASING CAMERAS");
  C0.release();
  C1.release();
  c0Open = false;
  c1Open = false;
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
    c0Open = true;
    logger -> info("Initialized");
  }
  else {
    c0Open = false;
    logger -> critical("Camera 0 failed to initialize");
  }
    
  logger -> info("Initializing camera 1...");
  C1.open(1);
  if (C1.isOpened()) {
    c1Open = true;
    logger -> info("Initialized");
  }
  else {
    c1Open = false;
    logger -> critical("Camera 1 failed to initialize");
  }
  return c0Open || c1Open;
}
