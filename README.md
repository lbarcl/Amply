# Amply

A digital guitar amplifier built with C++ and PortAudio, featuring real-time audio processing with gain and distortion effects.

## Features

- Real-time audio input/output processing
- Adjustable gain (0.0 - 10.0)
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

Once running, you can use the following commands. **Note:** These commands do **not** take arguments directly. Instead, they will prompt you to choose an option or input a value when executed.

- `gain` – Sets the gain multiplier.
- `rate` – Sets the sample rate.
- `input` – Selects the input device.
- `output` – Selects the output device.
- `start` – Starts audio processing.
- `stop` – Stops audio processing.
- `exit` – Exits the program.
- `help` – Displays this help message.
  
### Example

1. Connect your guitar or audio source to your computer.
2. Connect speakers or headphones to your computer.
3. Start the application.
4. Select connected ports.
5. Start the stream.
6. Adjust the gain and other effects.

## Architecture

- **DigitalAmp** – Core amplifier class handling audio processing.
- **CommandHandler** – Handles CLI commands and user input.
- **Effect** – Simple effect template for creating custom audio effects.
- **PortAudio** – Cross-platform audio I/O library (included as a submodule).
