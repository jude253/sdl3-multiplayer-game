cmake_minimum_required(VERSION 3.16)
project(woodeneye-008)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Add mongoose library and make it available to include
add_library(mongoose STATIC include/mongoose/mongoose.c include/mongoose/mongoose.h)
target_include_directories(mongoose PUBLIC include/mongoose)

# This assumes the SDL source is available in include/SDL
add_subdirectory(include/SDL EXCLUDE_FROM_ALL)

# on Web targets, we need CMake to generate a HTML webpage. 
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()


# Create your game executable target as usual
add_executable(woodeneye-008 src/woodeneye-008.c)

# Link libraries
target_link_libraries(woodeneye-008 PRIVATE SDL3::SDL3 mongoose)

# Dummy Server
add_executable(server src/server.c)
target_link_libraries(server PRIVATE mongoose)