#include "utils.h"
#include <iostream>
#include <string>
#include <vector>

// ===================== Host API Selection =====================

// Utility: pick best host API based on OS & fallback priority list
PaHostApiIndex chooseBestApi() 
{

#ifdef _WIN32
    std::vector<PaHostApiTypeId> priority = {
        paASIO, paWASAPI, paWDMKS, paDirectSound, paMME
    };
#elif __APPLE__
    std::vector<std::string> priority = { paCoreAudio };
#else // Linux & others
    std::vector<std::string> priority = {
        paPulseAudio, paALSA, paJACK, paOSS
    };
#endif

    int numApis = Pa_GetHostApiCount();
    if (numApis < 0) 
        return paHostApiNotFound;

    // Try APIs in order of priority
    for (const PaHostApiTypeId &pref : priority) 
    {
        for (int i = 0; i < numApis; i++) 
        {
            const PaHostApiInfo* apiInfo = Pa_GetHostApiInfo(i);
            if (apiInfo && apiInfo->type == pref) 
            {   
                return i;
            }
        }
    }

    // Fallback to default API
    return Pa_GetDefaultHostApi();
}

// ===================== Device Selection =====================

// Prompt user to select a device from a list
DeviceInfo promptForSelection(
    const std::string &prompt,
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

        if (line.empty()) 
            return defaultDevice;

        try 
        {
            int choice = std::stoi(line);
            if (choice >= 1 && choice <= static_cast<int>(devices.size()))
                return devices[choice - 1]; // convert to 0-based index
        } 
        catch (...) 
        {
            // invalid input, fall through
        }

        std::cout << "Invalid choice. Please enter a number between 1 and "
                  << devices.size() << ", or press Enter for default.\n";
    }
}

// ===================== Numeric Selection =====================

// Prompt user to select a numeric value (e.g., sample rates)
double promptForSelection(
    const std::string &prompt,
    const std::vector<double> &values,
    double defaultValue)
{
    if (values.empty()) 
    {
        std::cout << "  (No values available)\n";
        return defaultValue;
    }

    for (size_t i = 0; i < values.size(); i++) 
    {
        std::cout << "  " << (i + 1) << " - " << values[i] << "\n";
    }

    std::cout << "Press Enter to select default: " << defaultValue << "\n";

    while (true)
    {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) 
            return defaultValue;

        try 
        {
            int choice = std::stoi(line);
            if (choice >= 1 && choice <= static_cast<int>(values.size()))
                return values[choice - 1]; // 0-based index
        } 
        catch (...) 
        {
            // invalid input, fall through
        }

        std::cout << "Invalid choice. Please enter a number between 1 and "
                  << values.size() << ", or press Enter for default.\n";
    }
}
