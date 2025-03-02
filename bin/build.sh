#!/usr/bin/env bash

# This is a simple script to make it easier for me to build project
# in the same structure repeatedly

# echo "Script name: $0"
# echo "First argument: $1"
# echo "Second argument: $2"
# echo "Total number of arguments: $#"

BUILD_BASE_DIR="build"
NUMBER_OF_CORES=8  #Not sure how this will work on all computers


# Colors
GREEN="\033[0;32m"
CYAN="\033[0;36m"
NO_COLOR="\033[0m"


function setup_wasm {
  echo -e "${CYAN}Downloading Emscipten${NO_COLOR}"
  cd tool/emsdk
  ./emsdk install latest
  ./emsdk activate latest
  source ./emsdk_env.sh
  cd ../..
}


function build_non_wasm {
  echo -e "${GREEN}Building for non-wasm${NO_COLOR}"

  # Run build commands
  cmake -S . -B $BUILD_BASE_DIR/normal
  cmake --build $BUILD_BASE_DIR/normal -j${NUMBER_OF_CORES}
}


function build_wasm {
  echo -e "${GREEN}Building for wasm${NO_COLOR}"
  

  # Setup emsdk environment
  cd tool/emsdk
  source ./emsdk_env.sh
  cd ../..

  # Run build commands
  emcmake cmake -S . -B $BUILD_BASE_DIR/wasm
  emmake cmake --build $BUILD_BASE_DIR/wasm -j${NUMBER_OF_CORES}
}

function clean {
  echo -e "${GREEN}Running clean command${NO_COLOR}"

  rm -rf $BUILD_BASE_DIR
}


function print_usage {
  echo -e "${CYAN}USAGE:${NO_COLOR}"
  echo
  echo -e "bin/build.sh [setup_wasm,normal,wasm,clean]"
  echo
}

if [ "$1" = "setup_wasm" ]; then
  setup_wasm
fi

if [ "$1" = "normal" ]; then
  build_non_wasm
fi

if [ "$1" = "wasm" ]; then
  build_wasm
fi

if [ "$1" = "clean" ]; then
  clean
fi

if [ $# -eq 0 ]; then
  print_usage
fi
