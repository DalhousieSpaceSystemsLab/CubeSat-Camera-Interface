#!/usr/bin/env bash

RED='\033[0;31m'
RESET='\033[0m'

build='Build-Debug'

if (( $# == 0 )); then
  build='Build-Debug'
elif (( $# == 1 )); then
  if [ $1 == '-t' ]; then
    build='Build-Test'
  elif [ $1 == '-d' ]; then
    build='Build-Debug'
  else
    echo -e "${RED}[USAGE]${RESET} run.sh <-t><-d>"
    exit 1
  fi
fi
cd $build
./CubeSat_Payload
if (( $? != 0 )); then
  echo -e "${RED}[ERROR]${RESET} Program Crashed"
  exit 1
else
  cd ../
  exit 0
fi
