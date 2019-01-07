cmake_minimum_required( VERSION 2.8 )
project(main.cpp)
find_package( OpenCV REQUIRED )
add_executable(CubeSat_Payload main.cpp)
add_executable(CubeSat_Payload.exe main.cpp)
target_link_libraries( CubeSat_Payload.exe ${OpenCV_LIBS})
target_link_libraries( CubeSat_Payload main.cpp ${OpenCV_LIBS})
set(OPENCV_BUILD_3RDPARTY_LIBS)

