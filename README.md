
# CubeSat Camera
## Overview: 
This software will function as the interface between the camera modules (Color and NIR) and the operating system. The software will utilize the OpenCV library to capture photos from the two different cameras when run then save them in a specified area. We will also take into account various problems that may arise in a spatial environment by considering multiple countermeasures.  

### Objectives: 
- Capture two photos from two different cameras taking into account potential problems in a spatial environment. 
- Extremely fast picture capture to account for communication delay
- Ease of modification by operating system to account for potential changes in physical environment, such as disk space and camera damage.

### Language: 
- C++

### Target Operating System:
- Ubuntu Server 18.01

### Dependencies: 
- [OpenCV 4.0](https://opencv.org/)
  - Library of various Computer Vision interfaces and algorithms. 
  - Specifically, imgproc, imgcodecs, calib3d and VideoCapture modules will be used
- [CMake](https://cmake.org/)
  - Used to compile OpenCV with the main script
- *Dependencies may increase as development progresses*

### Command Line Args:
The following arguments are supported

Command | Purpose | Default
------------ | ------------- | -------------
-q | Runs the program without creating logs | 
-nc0 | Ignores camera 0 |
-nc1 | Ignores camera 1 |
-p * | Sets file path to * | Pictures/*date*
-n * | Sets file name to * | *date*
-c * | Sets file format to * | jpeg

