#ifndef LORIS_CAMERA
#define LORIS_CAMERA

#include <iostream> //printing to console (cerr)
#include <sys/stat.h> //mkdir
#include <ctime> //localtime

#include <opencv2/core.hpp> //core dependancies of opencv
#include <opencv2/videoio.hpp> //VideoCapture
#include <opencv2/imgcodecs.hpp> //imwrite

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "CameraParameters.hpp"

using namespace std;
using namespace cv;

class CubeSatCamera {
public:
    bool init();
    bool grab(cameraParams_t * param);
    bool release();
    bool isReady();
    void flush();
    void parseParams(vector<std::string> argv, cameraParams_t * param);
    string getFileName();
    CubeSatCamera() {
        try {
            logger = spdlog::daily_logger_mt("CubeSatCamera", "Error Logs/camera", 0, 0);
        } catch(spdlog::spdlog_ex) { //logger already exists
            logger = spdlog::get("CubeSatCamera");
        };
        spdlog::flush_every(chrono::seconds(5));
    }
    ~CubeSatCamera() {
        release();
        spdlog::drop("CubeSatCamera");
    }
private:
    int errorCount;
    bool c0Open = false;
    bool c1Open = false;
    std::shared_ptr<spdlog::logger> logger;
    VideoCapture C0 = NULL;
    VideoCapture C1 = NULL;
    VideoCapture * _C0 = &C0;
    VideoCapture * _C1 = &C1;
    string getDate();
    string getTime();
    Mat capture( VideoCapture *cap );
    bool grab(int camera, cameraParams_t * param);
    bool compress ( const Mat &frame, const cameraParams_t * param, string camera);
};
#endif

#ifndef CURRENT_TIME
#define CURRENT_TIME chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count()
#endif
