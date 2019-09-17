#ifndef LORIS_CAMERA_BUILD_TEST
#define LORIS_CAMERA_BUILD_TEST

#include "CubeSatCamera.hpp"
int demo(int argc, char const *argv[]);
string getTime();
void parseParams(vector<std::string> params, cameraParams_t * output);
#endif
