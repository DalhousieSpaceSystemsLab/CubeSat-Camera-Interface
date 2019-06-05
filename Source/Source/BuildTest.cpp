#include "CubeSatCamera.hpp"

int main(int argc, char const *argv[]) {
  long initTime;
  long camera0Time;
  long camera1Time;
  long startTime = CURRENT_TIME;
  string settings[argc] = {""};
  
  for (int i = 0; i < argc; i++) {
    settings[i] = argv[i];
  }

  CubeSatCamera camera(settings, argc);
  initTime = CURRENT_TIME - startTime;

  printf("enter any key to take a picture with camera 0: \n");
  getchar();

  startTime = CURRENT_TIME;
  camera.grab(0);
  camera0Time = CURRENT_TIME - startTime;

  printf("enter any key to take a picture with camera 1: \n");
  getchar();

  startTime = CURRENT_TIME;
  camera.grab(1);
  camera1Time = CURRENT_TIME - startTime;

  camera.log(NONE, "--------------------------------");
  camera.log(SYSTEM, "Runtimes: ");
  camera.log(SYSTEM, "Initialize Runtime: " + to_string(initTime) + "ms");
  camera.log(SYSTEM, "Camera0 Runtime: " + to_string(camera0Time) + "ms");
  camera.log(SYSTEM, "Camera1 Runtime: " + to_string(camera1Time) + "ms");
  camera.log(NONE, "--------------------------------");
  camera.log(NONE, "");
  string fileName = camera.getFileName();

  string pictureSize = "du -h Pictures/" + fileName + "*";
  string initTimeCmd = "echo 'Initialize Runtime: " + to_string(initTime) + "ms'";
  string cam0Cmd = "echo 'Camera0 Runtime: " + to_string(camera0Time) + "ms'";
  string cam1Cmd = "echo 'Camera1 Runtime: " + to_string(camera1Time) + "ms'";
  system("echo 'File sizes: '");
  system(pictureSize.c_str());
  system(initTimeCmd.c_str());
  system(cam0Cmd.c_str());
  system(cam1Cmd.c_str());
  return 0;
}
