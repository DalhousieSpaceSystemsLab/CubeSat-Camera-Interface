#ifndef LORIS_CAMERA
#define LORIS_CAMERA
#include <iostream> //printing to console (cerr)
#include <fstream> //creating a log file
#include <sys/stat.h> //mkdir
#include <ctime> //localtime
#include "CameraDataTypes.hpp"

#include <opencv2/core.hpp> //core dependancies of opencv
#include <opencv2/videoio.hpp> //VideoCapture
#include <opencv2/imgcodecs.hpp> //imwrite

#ifndef CURRENT_TIME
#define CURRENT_TIME chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count()
#endif

#ifndef SEVERE
#define SEVERE 1
#endif

#ifndef MODERATE
#define MODERATE 2
#endif

#ifndef ACTION
#define ACTION 11
#endif

#ifndef SYSTEM
#define SYSTEM 99
#endif

#ifndef NONE
#define NONE 999
#endif

using namespace std;
using namespace cv;


class CubeSatCamera {
public:
    bool init();
    bool grab(int camera, const std::string &filePath, const std::string &fileName, const std::string &compression, int quality);
    bool grab(cameraParams_t param);
    bool release();
    bool isReady();
    void log ( const int level, const string &msg );
    cameraParams_t parseParams(vector<std::string> argv);
    string getFileName();
    CubeSatCamera() {
        init();
    }
    ~CubeSatCamera() {
        release();
    }
private:
    int errorCount;
    ofstream printer;
    bool c0Open = true;
    bool c1Open = true;

    VideoCapture C0;
    VideoCapture C1;
    string getDate();
    string getTime();
    Mat capture( VideoCapture *cap );
    bool compress ( const Mat &frame, const std::string &path, const std::string &name, const std::string &compression, const int quality);
};
#endif