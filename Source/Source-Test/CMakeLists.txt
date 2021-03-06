cmake_minimum_required( VERSION 3.9.1 )
set ( CMAKE_CXX_STANDARD 11 )
find_package( OpenCV REQUIRED core videoio imgcodecs )
find_package( spdlog REQUIRED )

# Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_definitions("-DBUILD_SHARED_LIBS=1")

enable_testing()

include_directories( include ../Source/include)
add_executable( CubeSat_Payload ../Source/CubeSatCamera.cpp BuildTest.cpp )
target_link_libraries( CubeSat_Payload ${OpenCV_LIBS} )
target_Link_libraries( CubeSat_Payload spdlog::spdlog)
target_link_libraries( CubeSat_Payload gtest_main )
file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Error Logs")
file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Pictures")

add_test(CubeSat_Payload
         CubeSat_Payload)
