//
//  CubeSatCamera.cpp
//  CubeSatCamera
//
//  Created by Liam Hebert on 2018-12-21.
//  Copyright © 2018 Dal Orbital Science & Space Lab. All rights reserved.
//

#include "CubeSatCamera.hpp"

bool CubeSatCamera::isReady() {
  return C0.isOpened() || C1.isOpened();
}

//takes frame and saves it at path with specified compression format
bool CubeSatCamera::compress(const Mat &frame, const cameraParams_t *params, string camera) {
  long start_time = CURRENT_TIME;
  spdlog::debug("[compress()] STARTED - {}", CURRENT_TIME);
  vector<int> compression_params;
  if (params -> compression == "jpeg") {
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(params -> quality);
  }
  else if (params -> compression == "png") {
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(params -> quality);
  }
  bool result = imwrite(params -> filePath + params -> fileName + "(" + camera + ")" + "." + params -> compression, frame);
  spdlog::debug("[compress()] ENDED - {}", CURRENT_TIME);
  spdlog::debug("[compress()] Duration: {}", CURRENT_TIME - start_time);
  return result;
}

bool CubeSatCamera::grab(cameraParams_t * param) {
  long start_time = CURRENT_TIME;
  spdlog::debug("[grab() both Cameras] STARTED - {}", CURRENT_TIME);
  bool result = false;
  //if either is successful, return true;
  if (C0.isOpened()) 
    result = result || grab(0, param);
  if (C1.isOpened()) {
    result = result || grab(1, param);;
  }
  spdlog::debug("[grab() both Cameras] ENDED - {}", CURRENT_TIME);
  spdlog::debug("[grab() both Cameras] Duration: {}", CURRENT_TIME - start_time);
  return result;
}


bool CubeSatCamera::grab(int camera, cameraParams_t * param) {
  long start_time = CURRENT_TIME;
  spdlog::debug("[grab()] STARTED - {}", CURRENT_TIME);
  //talk about settings used
  VideoCapture * cam = NULL;
  Mat picture;

  logger -> info("Using Camera " + to_string(camera) + "...");
  if (camera == 0)
    cam = &C0;
  else if (camera == 1)
    cam = &C1;
  else {
    logger -> warn("Invalid camera selection");
    spdlog::debug("[grab()] ENDED - {}", CURRENT_TIME);
    spdlog::debug("[grab()] Duration: {}", CURRENT_TIME - start_time);
    return false;
  }
  
  *cam >> picture;
  if (!picture.empty()) {
    logger -> info("Frame grabbed successfully");
    if(compress(picture, param, "C" + to_string(camera))){
      logger -> info("Compressed successfully! Saved at " + param -> filePath + param -> fileName + " (C" + to_string(camera) + ")" + "." + param -> compression);
      logger -> info("Runtime: " + to_string(CURRENT_TIME - start_time));
      spdlog::debug("[grab()] ENDED - {}", CURRENT_TIME);
      spdlog::debug("[grab()] Duration: {}", CURRENT_TIME - start_time);
      return true;
    }
    else  //compression failed
      logger -> critical("Picture failed to compress");
  }
  else //camera not properly initialized
    logger -> warn("Unable to take picture");
  
  logger -> info("Runtime: " + to_string(CURRENT_TIME - start_time));
  spdlog::debug("[grab()] ENDED - {}", CURRENT_TIME);
  spdlog::debug("[grab()] Duration: {}", CURRENT_TIME - start_time);
  return false;
}


bool CubeSatCamera::release() {
  long start_time = CURRENT_TIME;
  spdlog::debug("[release()] STARTED - {}", CURRENT_TIME);
  logger -> info("RELEASING CAMERAS");
  C0.release();
  C1.release();
  logger -> flush();
  spdlog::debug("[release()] ENDED - {}", CURRENT_TIME);
  spdlog::debug("[release()] Duration: {}", CURRENT_TIME - start_time);
  return true;
}

void CubeSatCamera::flush() {
  logger -> flush();
}

bool CubeSatCamera::init() {
  long start_time = CURRENT_TIME;
  spdlog::debug("[init()] STARTED - {}", CURRENT_TIME);
  logger -> info("INITIALIZING CAMERAS");

  //initializing cameras here
  logger -> info("Initializing camera 0...");
  C0.open(0);
  if (C0.isOpened()) {
    logger -> info("Initialized");
  }
  else {
    logger -> critical("Camera 0 failed to initialize");
  }
  spdlog::debug("[C0] Initialization Duration: {}", CURRENT_TIME - start_time);
  logger -> info("Initializing camera 1...");
  C1.open(1);
  if (C1.isOpened()) {
    logger -> info("Initialized");
  }
  else {
    logger -> critical("Camera 1 failed to initialize");
  }
  spdlog::debug("[C1] Initialization Duration: {}", CURRENT_TIME - start_time);
  spdlog::debug("[init()] ENDED - {}", CURRENT_TIME);
  spdlog::debug("[init()] Duration: {}", CURRENT_TIME - start_time);
  return C0.isOpened() || C1.isOpened();
}
