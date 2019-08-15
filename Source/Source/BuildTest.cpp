#include "BuildTest.hpp"
using namespace std::chrono;

int demo(int argc, char const *argv[]) {
  cout << "== DEMO MODE ==" << endl;
  
  long initTime;
  long cameraTime;
  vector<std::string> settings;
  for (int i = 0; i < argc; i++) {
    settings.push_back(argv[i]);
  }
  CubeSatCamera camera;
  camera.release();

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  camera.init();
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto init = duration_cast<milliseconds>(t2 - t1).count();

  cameraParams_t params;
  camera.parseParams(settings, &params);
  
  cout << "enter any key to take a picture " << endl;
  getchar();
  t1 = high_resolution_clock::now();
  camera.grab(&params);
  t2 = high_resolution_clock::now();
  auto time = duration_cast<milliseconds>(t2 - t1).count();

  spdlog::info("Runtimes: ");
  spdlog::info("Initialize Runtime: {}ms", init);
  spdlog::info("Camera Runtime: {}ms", time);

  string pictureSize = "du -h Pictures/*";
  spdlog::info("File sizes: ");
  system(pictureSize.c_str());
  return 0;
}
