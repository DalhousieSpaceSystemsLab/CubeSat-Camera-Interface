#ifndef LORIS_CAMERA_DATA_TYPES
#define LORIS_CAMERA_DATA_TYPES
struct incoming_data {
    int initialize = 0;
    int shut_down = 1;
    int take_picture = 2;
    int change_settings = 3;
} incoming_data;

struct settings {
    int format = 10;
    int fileName = 11;
    int quality = 12;
    int filePath = 13;
} settings;

struct outgoing_data {
    int success = 0;
    int failure = 1;
    int initialized = 2;
    int file_name = 3;
} outgoing_data;

struct imageFormat {
    int jpeg = 0;
    int png = 1;
    int tiff = 2;
    int sr = 3;
} image_format;
#endif