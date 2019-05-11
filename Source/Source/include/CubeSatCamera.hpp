#include <iostream> //printing to console (cerr)
#include <fstream> //creating a log file
#include <sys/stat.h> //mkdir
#include <ctime> //localtime

#include <opencv2/core.hpp> //core dependancies of opencv
#include <opencv2/videoio.hpp> //VideoCapture
#include <opencv2/imgcodecs.hpp> //imwrite

#define CURRENT_TIME chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count()
#define SEVERE 1
#define MODERATE 2
#define ACTION 11
#define SYSTEM 99
#define NONE 999

using namespace std;
using namespace cv;


struct params_t {
    string date;
    bool quiet;
    string filePath;
    string fileName;
    string compression;
    int quality;
};


class CubeSatCamera {
public:
    bool init(string argv[], int argc);
    bool grab( int camera );
    bool release();
    void log ( const int level, const string &msg );

    string getFileName();
    CubeSatCamera(string argv[], int argc) {
        init(argv, argc);
    }
    ~CubeSatCamera() {
        release();
    }
private:
    int errorCount;
    ofstream printer;

    VideoCapture C0;
    VideoCapture C1;

    params_t param;
    string getDate();
    string getTime();
    Mat capture( VideoCapture *cap );
    bool compress ( const Mat &frame, const string &path, const string &name, const string &compression );
};
