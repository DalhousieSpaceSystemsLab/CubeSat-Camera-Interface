#include <iostream> //printing to console (cerr)
#include <fstream> //creating a log file
#include <sys/stat.h> //mkdir
#include <ctime> //localtime

#include <opencv2/core.hpp> //core dependancies of opencv
#include <opencv2/videoio.hpp> //VideoCapture
#include <opencv2/imgcodecs.hpp> //imwrite

using namespace std;
using namespace cv;
string getDate();
string getTime();
Mat capture(VideoCapture *cap);
bool compress(const Mat *frame, const string &path, const string &name, const string &compression);
void log(const int level, const string &msg);
