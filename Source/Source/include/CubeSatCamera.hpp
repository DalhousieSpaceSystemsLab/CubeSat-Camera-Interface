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

#ifndef CURRENT_TIME
#define CURRENT_TIME chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count()
#endif

using namespace std;
using namespace cv;

struct cameraParams_t {
    std::string date;
    std::string filePath;
    std::string fileName;
    std::string compression;
    int quality;
};

class CubeSatCamera {
public:
    bool init();
    bool grab(int camera, const std::string &filePath, const std::string &fileName, const std::string &compression, int quality);
    bool grab(cameraParams_t param);
    bool release();
    bool isReady();
    void flush();
    cameraParams_t parseParams(vector<std::string> argv);
    string getFileName();
    CubeSatCamera() {
        try {
            logger = spdlog::daily_logger_mt("CubeSatCamera", "Error Logs/camera", 0, 0);
        } catch(spdlog::spdlog_ex) {
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
    VideoCapture C0;
    VideoCapture C1;
    string getDate();
    string getTime();
    Mat capture( VideoCapture *cap );
    bool compress ( const Mat &frame, const std::string &path, const std::string &name, const std::string &compression, const int quality);
};
#endif