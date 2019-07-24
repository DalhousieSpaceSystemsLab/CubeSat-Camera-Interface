#ifndef LORIS_CAMERA_PARAMS
#define LORIS_CAMERA_PARAMS
#include <string>

struct cameraParams_t {
    std::string date;
    std::string filePath;
    std::string fileName;
    std::string compression;
    int quality;
    int operator==(const cameraParams_t param2) const {
        return date == param2.date
        && filePath == param2.filePath
        && fileName == param2.fileName 
        && compression == param2.compression
        && quality == param2.quality;
    };
};
#endif