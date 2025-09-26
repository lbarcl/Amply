# Amply

A digital guitar amplifier built with C++ and PortAudio, featuring real-time audio processing with gain and distortion effects.

## Features

- Real-time audio input/output processing
- Adjustable gain (0.0 - 10.0)
- Distortion effect with adjustable intensity (0.0 - 1.0)
- Interactive command-line interface
- Cross-platform support via PortAudio

## Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Git (for submodules)

## Building

1. Clone the repository with submodules:
   ```bash
   git clone --recursive https://github.com/lbarcl/Amply.git
   cd Amply
   ```
   
   If you already cloned without submodules, initialize them:
   ```bash
   git submodule update --init --recursive
   ```

2. Create build directory and compile:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the amplifier:
   ```bash
   ./bin/amply
   ```

## Usage

Once running, you can use these commands:
- `g <value>` - Set gain (e.g., `g 2.5`)
- `d <value>` - Set distortion (e.g., `d 0.7`)
- `h` - Show help
- `q` - Quit

## Hardware Setup

1. Connect your guitar or audio source to your computer's default audio input
2. Connect speakers or headphones to your computer's default audio output
3. Start the application and adjust gain and distortion as needed

## Architecture

- **DigitalAmp**: Core amplifier class handling audio processing
- **PortAudio**: Cross-platform audio I/O library (included as submodule)
- **Real-time Processing**: Low-latency audio callback system