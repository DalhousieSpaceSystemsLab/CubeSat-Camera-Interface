#include "CameraMessageInterface.hpp"
using namespace std;

//gets today's date in YYYY-MM-DD format
string getDate() {
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  string year = to_string(1900 + ltm -> tm_year);
  string month = to_string(1 + ltm -> tm_mon);
  string day = to_string(ltm -> tm_mday);
  return year + "-" + month + "-" + day;
}

bool sendStatus(int data) {
    MessageBuilder builder;
    builder.StartMessage();
    KeyValuePairContainer container;
    Identifiers identifiers;

    container.AddKeyValuePair(0, data);

    builder.SetSender(identifiers.payload_subsystem);
    builder.SetRecipient(identifiers.payload_repository);
    builder.SetMessageContents(container);

    Message message = builder.CompleteMessage();

    //MessageSenderInterface ms(message.GetRecipient());
    //ms.SendMessage(message);

    cout << endl << "== BEGIN CONTENTS ==" << endl;
	cout << endl << "RECIPIENT: " << message.GetRecipient() << endl
		<< "SENDER: " << message.GetSender() << endl
		<< "TIME CREATED: " << message.GetTimeCreated() << endl
		<< "CONTENTS:" << endl
        << INDENT_SPACES << "STATUS: " << message.GetInt(0) << endl;
    cout << endl << "== END CONTENTS ==" << endl << endl; 
    cout << "PROCESSING TIME: " << CURRENT_TIME - processingTime << "ms" << endl;
    return true;
}

bool sendPictureConfirmation(string filename) {
    spdlog::warn("UNDER CONSTRUCTION");
    MessageBuilder builder;
    builder.StartMessage();
    KeyValuePairContainer container;
    Identifiers identifers;

    spdlog::info("SENDING FILENAME {}", filename);
    container.AddKeyValuePair(0, outgoing_data.file_name);
    //container.AddKeyValuePair(1, filename); //pending string support

    builder.SetSender(identifers.payload_subsystem);
    builder.SetRecipient(identifers.payload_repository);
    builder.SetMessageContents(container);
    Message message = builder.CompleteMessage();

    cout << endl << "== BEGIN CONTENTS ==" << endl;
	cout << endl << "RECIPIENT: " << message.GetRecipient() << endl
		<< "SENDER: " << message.GetSender() << endl
		<< "TIME CREATED: " << message.GetTimeCreated() << endl
		<< "CONTENTS:" << endl
        << INDENT_SPACES << "FILENAME: " << filename << endl;
    cout << endl << "== END CONTENTS ==" << endl << endl; 

    //MessageSenderInterface ms(message.GetRecipient());
    //ms.SendMessage(message);
    return true;
}

int i = 0;
bool imgSwitch = false;
Message getMessage() {
    MessageBuilder builder;
    builder.StartMessage();
    KeyValuePairContainer container;
    Identifiers identifiers;
    if (i == 0) {
        spdlog::info("TEST: Sending INIT Message");
        container.AddKeyValuePair(0, incoming_data.initialize);
    } else if (i == 2) {
        spdlog::info("TEST: Sending TAKE PICTURE Message");
        container.AddKeyValuePair(0, incoming_data.take_picture);
    } else if (i == 4) {
        spdlog::info("TEST: Sending CHANGE SETTINGS Message");
        container.AddKeyValuePair(0, incoming_data.change_settings);
        if (imgSwitch) {
            spdlog::info("Switched -> png");
            container.AddKeyValuePair(1, image_format.png);
            imgSwitch = false;
        }
        else {
            spdlog::info("Switched -> jpeg");
            container.AddKeyValuePair(1, image_format.jpeg);
            imgSwitch = true;
        }
    } else if (i == 6) {
        spdlog::info("TEST: Sending SHUTDOWN Message");
        container.AddKeyValuePair(0, incoming_data.shut_down);
    } else if (i == 8) {
        spdlog::info("TEST: Resetting loop");
        i = -1; //will be set to 0 at the end of the loop
    } else {
        spdlog::info("TEST: Sending empty message");
    }
    i++;

    builder.SetMessageContents(container);
    builder.SetRecipient(identifiers.payload_subsystem);
    builder.SetSender(identifiers.payload_repository);
    Message message = builder.CompleteMessage();
    return message;
}

bool initializeCamera() {
    spdlog::info("INITIALIZING CAMERA");
    //due to short circuit operations, will check if ready first before camera.init()
    if (camera.isReady() || camera.init()) {
        spdlog::info("Ready to take picture");
        return true;
    }
    else {
        int i = 5;
        for (i = 5; i > 0; i--) {
            spdlog::warn("INTIALIZATION FAILED - Trying {} more times", i);
            camera.release();
            if (camera.init() == true) {
                spdlog::info("Ready to take picture");
                return true;
            }
        }
        spdlog::critical("INITIALIZATION TIMED OUT");
        return false;
    }
}

bool takePicture(cameraParams_t * param) {
    spdlog::info("STARTING TAKE PICTURE PROCESS");
    spdlog::info("Checking camera status...");
    if (camera.isReady()) {
        spdlog::info("Cameras are ready!");
    }
    else {
        spdlog::warn("Cameras were not ready!");
        spdlog::info("Initializing cameras...");
        if (initializeCamera()){
            spdlog::info("Cameras are ready to party!");
        }
        else {
            spdlog::critical("CAMERA FAILURE");
            return false;
        }
    }

    spdlog::info("Taking picture");
    if (camera.grab(param)) {
        spdlog::info("Picture taken!");
        return true;
    }
    spdlog::warn("PICTURE FAILED");
    return false;
}

bool shutdown() {
    spdlog::info("Shutting down cameras...");
    if (camera.isReady()) 
        camera.release();
    else 
        spdlog::warn("Cameras are already terminated");
    return true; //impossible for this method to fail
}

bool changeSettings(Message * message, cameraParams_t * params) {
    spdlog::info("Changing settings...");
    vector<int> keys;
    if (message -> GetMessageContents().GetAmountofIntPairs() > 1) {
        keys = message -> GetIntKeys();
        for (int i = 0; i < keys.capacity(); i++) {
            if(keys.at(i) == settings.format) {
                int format = message -> GetInt(settings.format);
                if (format == image_format.jpeg) 
                    params -> compression = "jpeg";
                else if (format == image_format.png) 
                    params -> compression = "png";
                else if (format == image_format.sr) 
                    params -> compression = "sr";
                else if (format == image_format.tiff)
                    params -> compression = "tiff";
                else {
                    spdlog::warn("Unknown image format");
                    spdlog::info("Keeping {} format", params -> compression);
                    return false;
                }
            } else if (keys.at(i) == settings.fileName) {
                spdlog::warn("UNDER CONSTRUCTION");
            } else if (keys.at(i) == settings.filePath) {
                spdlog::warn("UNDER CONSTRUCTION");
            } else if (keys.at(i) == settings.quality) {
                params -> quality = message -> GetInt(settings.quality);
            }
        }
        stringstream stream;
        stream << endl << "--------------------------------" << endl;
        stream << "SETTINGS" << endl;
        stream << "Date: " << getDate() << endl;
        stream << "File Path: " << params -> filePath << endl;
        stream << "File Name: " << params -> fileName << endl;
        stream << "Compression: " + params -> compression << endl;
        stream << "Quality Setting: " + to_string(params -> quality) << endl;
        stream << "--------------------------------" << endl;
        spdlog::info(stream.str());
        stream.clear();
    } 
    return true;
}


int main(int argc, char const *argv[]) { 
    if (argc > 1 && strcmp(argv[1],"-t") == 0) {
        camera = CubeSatCamera(0);
        demo(argc, argv);
        return 0;
    }
    camera = CubeSatCamera();
    spdlog::info("== NORMAL MODE ==");
    cameraParams_t params_struct;
    cameraParams_t * params = &params_struct;
    params -> compression = "jpeg";
    params -> date = getDate();
    params -> fileName = params -> date;
    params -> filePath = "Pictures/";
    params -> quality = 0;

    bool messageSuccess = false;
    while (true) {
        messageSuccess = false;
        spdlog::info("Checking for messages...");
        Message message = getMessage();
        if (message.GetMessageContents().GetAmountofIntPairs() != 0) {
            processingTime = CURRENT_TIME;
            spdlog::info("MESSAGE RECEIVED");
            spdlog::info("RECEIVED STATUS: {}", message.GetInt(0));
            if (message.GetInt(0) == incoming_data.initialize) 
                messageSuccess = initializeCamera();
            else if (message.GetInt(0) == incoming_data.take_picture) {
                if (takePicture(params)) {
                    messageSuccess = true;
                    sendPictureConfirmation(params -> fileName);
                }
            }
            else if (message.GetInt(0) == incoming_data.shut_down) 
                messageSuccess = shutdown();
            else if (message.GetInt(0) == incoming_data.change_settings) 
                messageSuccess = changeSettings(&message, params);
            if (messageSuccess) {
                spdlog::info("SUCCESS - Sending Success message");
                sendStatus(outgoing_data.success);
            }
            else {
                camera.flush();
                spdlog::critical("FAILURE - Sending Failure message");
                sendStatus(outgoing_data.failure);
            }
        } else {
            spdlog::info("No message recieved, standby...");
            this_thread::sleep_for(chrono::seconds(5));
        }
    }
    spdlog::critical("THIS SHOULD NOT HAPPEN!");
    return -1;
}