#include "utils.h"
#include <iostream>

// Utility: pick best host API based on OS & fallback list
PaHostApiIndex chooseBestApi() {
#ifdef _WIN32
    std::vector<std::string> priority = {
        "Windows WASAPI", "Windows WDM-KS", "Windows DirectSound", "MME"
    };
#elif __APPLE__
    std::vector<std::string> priority = { "Core Audio" };
#else // Linux & others
    std::vector<std::string> priority = {
        "PulseAudio", "ALSA", "JACK Audio Connection Kit", "OSS"
    };
#endif

    int numApis = Pa_GetHostApiCount();
    if (numApis < 0) return paHostApiNotFound;

    // Try APIs in order of priority
    for (auto &pref : priority) {
        for (int i = 0; i < numApis; i++) {
            const PaHostApiInfo* apiInfo = Pa_GetHostApiInfo(i);
            if (std::string(apiInfo->name).find(pref) != std::string::npos) {
                return i;
            }
        }
    }

    // fallback to default API
    return Pa_GetDefaultHostApi();
}

DeviceInfo promptForSelection(const std::string &prompt,
                              const std::vector<DeviceInfo> &devices,
                              const DeviceInfo &defaultDevice)
{
    if (devices.empty())
    {
        std::cout << "  (No devices found)\n";
        return defaultDevice;
    }

    for (size_t i = 0; i < devices.size(); i++)
    {
        std::cout << "  " << (i + 1) << " - " << devices[i].name
                  << " (in: " << devices[i].maxInputChannels
                  << ", out: " << devices[i].maxOutputChannels << ")\n";
    }

    std::cout << "Press Enter to select default: " << defaultDevice.name << "\n";

    while (true)
    {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            return defaultDevice;
        }

        try {
            int choice = std::stoi(line);
            if (choice >= 1 && choice <= static_cast<int>(devices.size()))
            {
                return devices[choice - 1]; // convert back to 0-based index
            }
        } catch (...) {
            // invalid conversion
        }

        std::cout << "Invalid choice. Please enter a number between 1 and "
                  << devices.size() << ", or press Enter for default.\n";
    }
}

double promptForSelection(const std::string &prompt,
                          const std::vector<double> &values,
                          double defaultValue)
{
    if (values.empty()) {
        std::cout << "  (No values available)\n";
        return defaultValue;
    }

    for (size_t i = 0; i < values.size(); i++) {
        std::cout << "  " << (i + 1) << " - " << values[i] << "\n";
    }

    std::cout << "Press Enter to select default: " << defaultValue << "\n";

    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            return defaultValue;
        }

        try {
            int choice = std::stoi(line);
            if (choice >= 1 && choice <= static_cast<int>(values.size())) {
                return values[choice - 1]; // 0-based index
            }
        } catch (...) {
            // invalid conversion
        }

        std::cout << "Invalid choice. Please enter a number between 1 and "
                  << values.size() << ", or press Enter for default.\n";
    }
}
