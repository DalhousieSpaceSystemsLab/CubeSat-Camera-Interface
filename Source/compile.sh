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

cmakeBuild() {
  echo -e "${BYELLOW}Building new build...${RESET}"
  echo ""
  cmake -B Build-Debug -S Source
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
  cmake --build ./Build-Debug
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


if (( $# > 1 )); then
  echo -e "${RED}[USAGE]${RESET} compile.sh <mode>"
  exit 2
fi

if (( $# == 0 )); then
  cmakeBuild
  if (( $cmakeSuccess == 1 )); then
    compile
  fi
  results
fi

mode=$1;
if (( mode == "-f" )); then
  echo -e "${BYELLOW}Cleaning old files...${RESET}"
  echo ""
  echo -e "${YELLOW}Deleting Build-Debug/CMakeCache.txt...${RESET}"
  rm Build-Debug/CMakeCache.txt
  echo -e "${YELLOW}Deleting Build-Debug/CubeSat_Payload...${RESET}"
  rm Build-Debug/CubeSat_Payload
  echo -e "${YELLOW}Deleting Build-Debug/Makefile...${RESET}"
  rm Build-Debug/Makefile
  echo -e "${YELLOW}Deleting Build-Debug/cmake_install.cmake...${RESET}"
  rm Build-Debug/cmake_install.cmake
  echo ""
  echo -e "${GREEN}[OK]${RESET} All clean!"
  echo ""

  cmakeBuild
  if (( $cmakeSuccess == 1 )); then
    compile
  fi
  results

fi
