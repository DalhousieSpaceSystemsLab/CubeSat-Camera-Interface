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
    
}

bool sendPictureConfirmation(string filename) {

}

Message getMessage() {

}

bool initializeCamera() {
    cout << "INITIALIZING CAMERA";
    //due to short circuit operations, will check if ready first before camera.init()
    if (camera.isReady() || camera.init()) {
        cout << "Ready to take picture";
        return true;
    }
    else {
        int i = 5;
        for (i = 5; i > 0; i--) {
            cout << "INTIALIZATION FAILED - Trying " << i << " more times";
            camera.release()
            if (camera.init() == true) {
                cout << "Ready to take picture";
                return true;
            }
        }
        cout << "INITIALIZATION TIMED OUT";
        return false;
    }
}

bool takePicture(camera_params_t param) {
    cout << "STARTING TAKE PICTURE PROCESS";
    cout << "Checking camera status...";
    if (camera.isReady()) {
        cout << "Cameras are ready!";
    }
    else {
        cout << "[WARN] - Cameras were not ready!"
        cout << "Initializing cameras...";
        if (initializeCamera()){
            cout << "Cameras are ready to party!"
        }
        else {
            cout << "CAMERA FAILURE";
            return false;
        }
    }

    cout << "Taking picture";
    if (camera.grab(param)) {
        cout << "Picture taken!"
        return true;
    }
    cout << "PICTURE FAILED";
    return false;
}

bool shutdown() {
    cout << "Shutting down cameras...";
    if (camera.isReady()) 
        camera.release();
    else 
        cout << "Cameras are already terminated";
    return true; //impossible for this method to fail
}

bool changeSettings(Message * message, cameraParams_t * params) {
    cout << "Changing settings...";
    vector<int> keys;
    if (message -> GetMessageContents().GetAmountofIntPairs() > 1) {
        keys = message -> GetIntKeys();
        
    } 
}


int main() {   
    cout << "== NORMAL MODE ==" << endl;
    camera.release();
    cameraParams_t params_struct;
    cameraParams_t * params = &params_struct;
    params -> compression = "jpeg";
    params -> date = getDate();
    params -> fileName = param.date;
    params -> filePath = "Pictures/";
    params -> quality = 0;

    bool messageSuccess = false;
    while (true) {
        messageSuccess = false;
        cout << "Checking for messages..." << endl;
        Message message = getMessage();
        if (message.GetMessageContents().GetAmountofIntPairs() != 0) {
            cout << "MESSAGE RECEIVED" << endl;
            cout << "RECEIVED STATUS: " << message.GetInt(0);
            if (message.GetInt(0) == incoming_data.initialize) 
                messageSuccess = initializeCamera();
            else if (message.GetInt(0) == incoming_data.take_picture) {
                messageSuccess = takePicture(params);
            else if (message.GetInt(0) == incoming_data.shut_down) {
                messageSuccess = shutdown();
            }
            else if (message.GetInt(0) == incoming_data.change_settings) {
                messageSuccess = changeSettings(&message, params);
            }
            if (messageSuccess) {
                cout << "SUCCESS - Sending Success message";
                sendStatus(outgoing_data.success);
            }
            else {
                cout << "FAILURE - Sending Failure message";
                sendStatus(outgoing_data.failure);
            }
        }
    }
}