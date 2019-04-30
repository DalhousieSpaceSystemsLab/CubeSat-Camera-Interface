#!/usr/bin/env bash

RED='\033[0;31m'
RESET='\033[0m'

cd Build-Debug
./CubeSat_Payload
if (( $? != 0 )); then
  echo -e "${RED}[ERROR]${RESET} Program Crashed, open error logs folder..."
  cd Error\ Logs/
  exit 1
else
  cd ../
  exit 0
fi
