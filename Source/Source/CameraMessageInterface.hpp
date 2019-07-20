#include "include/CubeSatCamera.hpp"
#include "CameraDataTypes.hpp"
#include "Message.h"
#include "MessageBuilder.h"
#include <string>
#include <iostream>
#include <math.h>
#include <chrono>
#include "Identifiers.h"

bool sendStatus(int data);
bool sendPictureConfirmation(string data);
Message getMessage();
int main();
string getDate();
CubeSatCamera camera;