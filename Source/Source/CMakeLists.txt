cmake_minimum_required( VERSION 3.9.1 )
set ( CMAKE_CXX_STANDARD 14 )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Build-Debug)
set (CMAKE_BINARY_DIR ${CMAKE_HOME_DIRECTORY}/../Build-Debug)
project( CubeSatCamera.cpp )
find_package( OpenCV REQUIRED )
find_package( spdlog REQUIRED )

SET ( OS include/internal-messaging)

SET ( unixdomain

    ${OS}/unixdomain/cc/UnixDomainStreamSocket.cc 
    ${OS}/unixdomain/cc/UnixDomainStreamSocketClient.cc 
    ${OS}/unixdomain/cc/UnixDomainStreamSocketServer.cc 
)

SET ( message 
   ${OS}/messaging-service/cc/MessageRecievingService.cc
   ${OS}/messaging-service/cc/PhoneBook.cc 
   
   ${OS}/message/cc/MessageBuilder.cc
   ${OS}/message/cc/Message.cc 
   ${OS}/message/cc/DataMessage.cc
   ${OS}/message/cc/CommandMessage.cc
   ${OS}/message/cc/KeyValuePairContainer.cc 
   ${OS}/message/cc/Key.cc 
   ${OS}/message/cc/KeyIntPair.cc 
   ${OS}/message/cc/KeyFloatPair.cc
   ${OS}/message/cc/KeyValuePairContainerInterface.cc 
   ${OS}/message/cc/Message.cc 
   ${OS}/message/cc/MessageBuilder.cc 
   ${OS}/message/cc/MessageBuilderInterface.cc
)

include_directories ( 

   include 
   ${OS}/message/header 
   ${OS}/message/identifiers 
   ${OS}/unixdomain/header 
   ${OS}/messaging-service/header
)  

add_executable( CubeSat_Payload 

   CubeSatCamera.cpp 
   BuildTest.cpp 
   CameraMessageInterface.cpp 
   ${unixdomain} 
   ${message}
)

target_link_libraries( CubeSat_Payload ${OpenCV_LIBS} spdlog::spdlog )
file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Error Logs")
file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Pictures")