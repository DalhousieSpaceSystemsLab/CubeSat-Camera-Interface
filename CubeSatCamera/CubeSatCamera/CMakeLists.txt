cmake_minimum_required( VERSION 2.8 )
project( main.cpp )
set (CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
add_executable(CubeSat_Payload main.cpp)
target_link_libraries( CubeSat_Payload ${OpenCV_LIBS})

