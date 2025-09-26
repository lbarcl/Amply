cmake_minimum_required(VERSION 3.15)
project(Amply VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add external dependencies directory
add_subdirectory(external/portaudio)

# Include directories
include_directories(include)
include_directories(external/portaudio/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(amply ${SOURCES})

# Link libraries
target_link_libraries(amply portaudio)

# Set output directory
set_target_properties(amply PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)