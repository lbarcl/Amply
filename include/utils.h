#pragma once
#include <vector>
#include <string>
#include <portaudio.h>

// A safe copy of device information
struct DeviceInfo
{
    int index;             // PortAudio device index
    std::string name;      // Device name
    int maxInputChannels;  // >0 means input
    int maxOutputChannels; // >0 means output
};

// A container of available devices
struct AvailableDevices
{
public:
    std::vector<DeviceInfo> inputs;
    std::vector<DeviceInfo> outputs;
};

PaHostApiIndex chooseBestApi();

DeviceInfo promptForSelection(const std::string &prompt,
                              const std::vector<DeviceInfo> &devices,
                              const DeviceInfo &defaultDevice);

double promptForSelection(const std::string &prompt,
                          const std::vector<double> &values,
                          double defaultValue);