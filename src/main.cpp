#include "digitalamp.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    std::cout << "Starting Amply Digital Amplifier...\n";

    DigitalAmp amp;

    // Initialize the amplifier
    if (!amp.initialize())
    {
        std::cerr << "Failed to initialize amplifier" << std::endl;
        return 1;
    }

    auto devices = amp.getAvailableDevices();

    std::cout << "İnput devices:" << std::endl;

    for (int i = 0; i < devices->inputs.size(); i++)
    {
        DeviceInfo info = devices->inputs[i];
        std::cout << info.index << " - " << info.name << std::endl;
    }

    std::cout << "Output devices:" << std::endl;

    for (int i = 0; i < devices->outputs.size(); i++)
    {
        DeviceInfo info = devices->outputs[i];
        std::cout << info.index << " - " << info.name << std::endl;
    }
}