#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BYELLOW='\033[1;33m'
BRED='\033[1;31m'
BGREEN='\033[1;32m'
RESET='\033[0m' # No Color
cmakeSuccess=0
compileSuccess=0
build='Build-Debug'
source='Source'
fresh=0

clean() {
  echo -e "${BYELLOW}Cleaning old files...${RESET}"
  echo ""
  echo -e "${YELLOW}Deleting $build/CMakeCache.txt...${RESET}"
  rm $build/CMakeCache.txt
  echo -e "${YELLOW}Deleting $build/CubeSat_Payload...${RESET}"
  rm $build/CubeSat_Payload
  echo -e "${YELLOW}Deleting $build/Makefile...${RESET}"
  rm $build/Makefile
  echo -e "${YELLOW}Deleting $build/cmake_install.cmake...${RESET}"
  rm $build/cmake_install.cmake
  echo ""
  echo -e "${GREEN}[OK]${RESET} All clean!"
  echo ""
}

cmakeBuild() {
  echo -e "${BYELLOW}Building new build...${RESET}"
  echo ""
  cmake -B $build -S $source
  result=$?
  echo ""
  if (( $result != 0 )); then
    echo -e "${RED}[ERROR]${RESET} CMake process failed";
    cmakeSuccess=0
  else
    echo -e "${GREEN}[OK]${RESET} CMake Success!"
    echo ""
    cmakeSuccess=1
  fi
}

compile() {
  echo -e "${BYELLOW}Compiling...${RESET}"
  echo ""
  cmake --build ./$build
  result=$?
  echo ""
  if (( $result != 0 )); then
    echo -e "${RED}[ERROR]${RESET} Compile process failed";
    compileSuccess=0
  else
    echo -e "${GREEN}[OK]${RESET} Compile Success!"
    echo ""
    compileSuccess=1
  fi
}

results() {
  echo -e "${BYELLOW}- RESULTS - ${RESET}"
  if (( $cmakeSuccess == 0 )); then
    echo -e "${RED}[ERROR]${RESET} CMake process failed";
  else
    echo -e "${GREEN}[OK]${RESET} CMake Success!"
  fi

  if (( $compileSuccess == 0 )); then
    echo -e "${RED}[ERROR]${RESET} Compile process failed";
  else
    echo -e "${GREEN}[OK]${RESET} Compile Success!"
  fi

  if (( $compileSuccess + $cmakeSuccess == 2 )); then
    echo -e "${BGREEN}Full Success!${RESET}"
    exit 0;
  else
    echo -e "${BRED}Unfortuately, Full Failure${RESET}"
    exit 1;
  fi
}


if (( $# > 2 )); then
  echo -e "${RED}[USAGE]${RESET} compile.sh <-b><-f>"
  exit 2
fi

if (( $# == 0 )); then
  cmakeBuild
  if (( $cmakeSuccess == 1 )); then
    compile
  fi
  results
else
  for var in "$@"; do
    if [ $var == "-f" ]; then
      fresh=1
    elif [ $var == '-t' ]; then
      build='Build-Test'
      source='Source-Test'
    else
      echo -e "${RED}[USAGE]${RESET} compile.sh <-t><-f>"
      exit 2
    fi
  done
fi

if (( $fresh == 1 )); then
  clean
fi

cmakeBuild
if (( $cmakeSuccess == 1 )); then
  compile
fi
  results
fi
