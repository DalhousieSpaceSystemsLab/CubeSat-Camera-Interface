#include "BuildTest.hpp"
using namespace std::chrono;

int demo(int argc, char const *argv[]) {
  cout << "== DEMO MODE ==" << endl;
  
  vector<std::string> settings;
  for (int i = 0; i < argc; i++) {
    settings.push_back(argv[i]);
  }
  CubeSatCamera camera(0);
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  camera.init();
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto init = duration_cast<milliseconds>(t2 - t1).count();

  cameraParams_t params;
  parseParams(settings, &params);
  
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

void parseParams(vector<std::string> params, cameraParams_t * output) {
  //gathering commandline args
  string arg;

  //default params
  output -> compression = "jpeg";
  output -> date = getTime();
  output -> fileName = output -> date;
  output -> filePath = "Pictures/";
  output -> quality = 70;
  

  for (int i = 0; i < params.size(); i++) { //sadly switch statements cannot be used here without hashing, which has the potential to be inaccurate and is not worth the time loss
    arg = params.at(i);
    if (arg == "-p") { //custom path
      if (i + 1 < params.size()) { //room for filename
        arg = params.at(i+1);
        if (arg.at(0) != '-') { //valid format
          output -> filePath = arg;
        }
        else {
          cerr << "invalid path format" << endl;
        }
      } else cerr << "missing path string" << endl;
      continue;
    }
    else if (arg == "-n") { //custom filename
      if (i + 1 < params.size()) {
        arg = params.at(i + 1);
        if (arg.at(0) != '-') {
          output -> fileName = arg;
          i++;
        }
        else 
          cerr << "invalid file name format" << endl;
      } else cerr << "missing file name string" << endl;
      continue;
    }
    else if (arg == "-c") { //custom compression
      if (i + 1 < params.size()) {
        arg = params.at(i + 1);
        if (arg.at(0) != '-') {
          output -> compression = arg;
          if (arg == "jpeg" && output -> quality == 0) 
            output -> quality = 90;
          else if (arg == "png" && output -> quality == 0)
            output -> quality = 8;
          i++;
        }
        else 
          cerr << "invalid compression format" << endl;
      } else cerr << "missing compression string" << endl;
      continue;
    }
    else if (arg == "-iq") {
      if (i + 1 < params.size()) {
        arg = params.at(i + 1);
        if (arg.at(0) != '-') {
          output -> quality = stoi(arg); //will check what the real format is later
          i++;
        }
        else 
          cerr << "invalid quality format" << endl;
      } else cerr << "missing compression string" << endl;
      continue;
    }
  }
}

  //gets today's date in YYYY-MM-DD format
string getTime() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  string year = to_string(1900 + ltm -> tm_year);
  string month = to_string(1 + ltm -> tm_mon);
  string day = to_string(ltm -> tm_mday);
  return year + "-" + month + "-" + day;
}

