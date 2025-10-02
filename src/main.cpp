#include "digitalamp.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// helper: safe device selection
DeviceInfo promptForSelection(const std::string &prompt,
                              const std::vector<DeviceInfo> &devices)
{
    if (devices.empty())
    {
        std::cout << "  (No devices found)\n";
        return DeviceInfo{0};
    }

    for (size_t i = 0; i < devices.size(); i++)
    {
        std::cout << "  " << (i + 1) << " - " << devices[i].name
                  << " (in: " << devices[i].maxInputChannels
                  << ", out: " << devices[i].maxOutputChannels << ")\n";
    }

    int choice = -1;
    while (true)
    {
        std::cout << prompt;
        std::cin >> choice;

        if (!std::cin)
        {
            // clear bad input (like letters instead of numbers)
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice >= 1 && choice <= static_cast<int>(devices.size()))
        {
            return devices[choice - 1]; // convert back to 0-based index
        }

        std::cout << "Invalid choice. Please enter between 1 and "
                  << devices.size() << ".\n";
    }
}

int main(int argc, char *argv[])
{
    std::cout << "Starting Amply Digital Amplifier...\n";

    DigitalAmp amp;

    if (!amp.initialize())
    {
        std::cerr << "Failed to initialize amplifier\n";
        return 1;
    }

    auto devices = amp.getAvailableDevices();

    std::cout << "\n=== Input Devices ===\n";
    DeviceInfo inputDevice = promptForSelection("Select input device: ", devices->inputs);

    std::cout << "\n=== Output Devices ===\n";
    DeviceInfo outputDevice = promptForSelection("Select output device: ", devices->outputs);

    if (inputDevice.index == -1 || outputDevice.index == -1)
    {
        std::cerr << "Input or output device not selected. Exiting.\n";
        return 1;
    }

    std::cout << "Selected input: " << inputDevice.name << inputDevice.index << "\n";
    std::cout << "Selected output: " << outputDevice.name << outputDevice.index << "\n";

    int commonChannelCount = amp.chooseCommonChannelCount(inputDevice, outputDevice);

    if (commonChannelCount == 0)
    {
        std::cerr << "No valid channel count found!\n";
        return 1;
    }

    if (amp.createStreamParameters(inputDevice.index, commonChannelCount, paFloat32, true))
        return 1;

    if (amp.createStreamParameters(outputDevice.index, commonChannelCount, paFloat32, false))
        return 1;

    double sampleRate = amp.choseBestSampleRate();

    if (!amp.openStream(sampleRate, paFramesPerBufferUnspecified))
        return 1;

    amp.startStream();

    std::cout << "Streaming at " << sampleRate << " Hz, "
              << commonChannelCount << " channels. Press Enter to stop.\n";

    int a;
    std::cin >> a;

    amp.stopStream();

    return 0;
}
