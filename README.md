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
1. Setup [Emscripten/emsdk](https://emscripten.org/docs/getting_started/downloads.html) (commands adapted from official install instructions and 
assuming all submodules are present)

    ```bash
    cd tool/emsdk
    ./emsdk install latest
    ./emsdk activate latest
    source ./emsdk_env.sh
    cd ../..
    ```

## Build Commands

I added a simple bash build script at `bin/build.sh` to make the build
commands more easily repeatible for me.  I am thinking something like
bazel might be better eventually, but for now this is more systematic.


NOTE: speed up build by adding -j<number_of_cores> to build command. 
Ex: `cmake --build build -j8`


### Normal Executable
```bash
cmake -S . -B build
cmake --build build
```

Build approach was modeled after this: https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md

#### Run Command

After building, run: 
```bash
./build/woodeneye-008
```


### WASM

First time in terminal session run: `source ./emsdk_env.sh`

```bash
emcmake cmake -S . -B build
emmake cmake --build build
```

Build approach modeled after this: https://github.com/libsdl-org/SDL/blob/main/docs/README-emscripten.md#building-sdlemscripten

#### Run Command

After building, run: 
```bash
emrun build/woodeneye-008.html
```


# TODO
- Build with Bazel?
  - Use: https://github.com/bazel-contrib/rules_foreign_cc ?