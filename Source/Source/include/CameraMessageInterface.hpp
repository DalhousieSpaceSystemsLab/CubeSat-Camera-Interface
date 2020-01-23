#ifndef LORIS_CAMERA_MESSAGE_INTERFACE
#define LORIS_CAMERA_MESSAGE_INTERFACE

#include "CubeSatCamera.hpp"
#include "BuildTest.hpp"
#include "CameraDataTypes.hpp"
#include "Message.h"
#include "DataMessage.h"
#include "PhoneBook.h"
#include "CommandMessage.h"
#include "MessageBuilder.h"
#include "MessageReceivingService.h"
#include <string>
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include "Identifiers.h"

bool sendStatus(int data);
bool sendPictureConfirmation(string data);
CommandMessage getMessage();
int main(int argc, char const *argv[]);
string getDate();
std::shared_ptr<spdlog::logger> logger;
CubeSatCamera camera = NULL;
long processingTime;


#endif 

#ifndef INDENT_SPACES
#define INDENT_SPACES "  "
#endif 