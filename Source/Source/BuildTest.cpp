#include "BuildTest.hpp"

int demo(int argc, char const *argv[]) {
  cout << "== DEMO MODE ==" << endl;
  
  long initTime;
  long cameraTime;
  vector<std::string> settings;
  for (int i = 0; i < argc; i++) {
    settings.push_back(argv[i]);
  }
  CubeSatCamera camera;

  long startTime = CURRENT_TIME;
  camera.init();
  initTime = CURRENT_TIME - startTime;

  cameraParams_t params;
  camera.parseParams(settings, &params);
  
  cout << "enter any key to take a picture " << endl;
  getchar();
  startTime = CURRENT_TIME;
  camera.grab(&params);
  cameraTime = CURRENT_TIME - startTime;

  spdlog::info("Runtimes: ");
  spdlog::info("Initialize Runtime: " + to_string(initTime) + "ms");
  spdlog::info("Camera Runtime: " + to_string(cameraTime) + "ms");

  string pictureSize = "du -h Pictures/*";
  spdlog::info("File sizes: ");
  system(pictureSize.c_str());
  return 0;
}
