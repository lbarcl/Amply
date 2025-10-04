#pragma once
#include <vector>
#include <string>
#include <portaudio.h>

// ===================== Device Information =====================

// A safe copy of PortAudio device information
struct DeviceInfo
{
    int index;             // PortAudio device index
    std::string name;      // Device name
    int maxInputChannels;  // >0 means input-capable
    int maxOutputChannels; // >0 means output-capable
};

// Container for available devices
struct AvailableDevices
{
    std::vector<DeviceInfo> inputs;   // All input devices
    std::vector<DeviceInfo> outputs;  // All output devices
};

// ===================== Utility Functions =====================

// Returns the most suitable PortAudio host API for the system
PaHostApiIndex chooseBestApi();

// Prompts the user to select a device from a list
DeviceInfo promptForSelection(
    const std::string &prompt,
    const std::vector<DeviceInfo> &devices,
    const DeviceInfo &defaultDevice);

// Prompts the user to select a numeric value from a list (e.g., sample rates)
double promptForSelection(
    const std::string &prompt,
    const std::vector<double> &values,
    double defaultValue);
