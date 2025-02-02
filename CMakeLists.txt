cmake_minimum_required(VERSION 3.16)
project(woodeneye-008)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(include/SDL EXCLUDE_FROM_ALL)

# Copy html file to build folder in case compiling for WASM:
configure_file("public/woodeneye-008.html" "woodeneye-008.html")


# Create your game executable target as usual
add_executable(woodeneye-008 src/woodeneye-008.c)

# Link to the actual SDL3 library.
target_link_libraries(woodeneye-008 PRIVATE SDL3::SDL3)