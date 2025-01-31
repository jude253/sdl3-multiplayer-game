# Overview

This is a small repo that uses [SDL3](https://wiki.libsdl.org/SDL3)
and runs one of the demos as if that was the code that I wrote for this
project.

## Dependencies

- CMake
  - Docs: https://cmake.org/
  - Installation: https://github.com/Kitware/CMake?tab=readme-ov-file#unixmac-osxmingwmsyscygwin
- SDL3
  - Docs: https://wiki.libsdl.org/SDL3
  - Source Code: https://github.com/libsdl-org/SDL
  - Installation: https://github.com/libsdl-org/SDL/blob/main/INSTALL.md
- Emscripten
  - Docs: https://emscripten.org/
  - Source Code: https://github.com/emscripten-core/emsdk
  - Installation: https://emscripten.org/docs/getting_started/downloads.html

## Install Project

1. Clone this repo including submodules 
    ```bash
    # Clone repo and submodules:
    git clone --recurse-submodules https://github.com/jude253/sdl3-demo-game.git
    ```


## Build Commands

```bash
cmake -S . -B build
cmake --build build
```

Build approach was modeled after this: https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md

## Run Command

After building, run: 
```bash
./build/woodeneye-008
```


# TODO
- Add Emscripten setup/commands to build for WASM