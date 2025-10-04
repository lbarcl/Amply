#include "commandhandler.h"
#include "digitalamp.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include <unordered_map>
#include <functional>

// ===================== Constructor =====================
CommandHandler::CommandHandler(DigitalAmp *amp) : amp(amp)
{
    // Initialize command table
    commands = {
        {"help", [this] { showHelp(); }},
        {"input", [this] { chooseInput(); }},
        {"output", [this] { chooseOutput(); }},
        {"rate", [this] { chooseSampleRate(); }},
        {"start", [this] { startStream(); }},
        {"stop", [this] { closeStream(); }},
        {"exit", [this] { exitApp(); }},
        {"clear", [this] { clearConsole(); }}
    };
}

// ===================== Main Loop =====================
void CommandHandler::run()
{
    std::cout << "Welcome to Amply Digital Amplifier!\n";
    std::cout << "Type 'help' to see available commands.\n";

    while (true)
    {
        std::cout << "\n> ";
        std::string cmdStr;
        std::cin >> cmdStr;
        clearInputBuffer();

        auto it = commands.find(cmdStr);
        if (it != commands.end())
        {
            it->second(); // Execute command
        }
        else
        {
            std::cout << "Unknown command.\n";
            showHelp();
        }
    }
}

// ===================== Device Selection =====================
void CommandHandler::chooseInput()
{
    auto devices = amp->getAvailableDevices();
    auto defaultDevice = amp->getDefaultDevice(true);

    DeviceInfo selectedDevice = promptForSelection(
        "Select input device: ", devices->inputs, defaultDevice);

    amp->createStreamParameters(
        selectedDevice.index,
        selectedDevice.maxInputChannels,
        paFloat32,
        true);

    std::cout << "[Info] Selected input device: "
              << selectedDevice.name
              << " (" << selectedDevice.maxInputChannels << " channels)\n";
}

void CommandHandler::chooseOutput()
{
    auto devices = amp->getAvailableDevices();
    auto defaultDevice = amp->getDefaultDevice(false);

    DeviceInfo selectedDevice = promptForSelection(
        "Select output device: ", devices->outputs, defaultDevice);

    amp->createStreamParameters(
        selectedDevice.index,
        selectedDevice.maxOutputChannels,
        paFloat32,
        false);

    std::cout << "[Info] Selected output device: "
              << selectedDevice.name
              << " (" << selectedDevice.maxOutputChannels << " channels)\n";
}

// ===================== Sample Rate Selection =====================
void CommandHandler::chooseSampleRate()
{
    amp->sampleRate = promptForSelection(
        "Select the sample rate: ",
        amp->getSupportedSampleRates(),
        amp->choseBestSampleRate());

    std::cout << "[Info] Selected sample rate: "
              << amp->sampleRate << " Hz\n";
}

// ===================== Stream Control =====================
void CommandHandler::startStream()
{
    if (amp->sampleRate == 0.0)
        amp->sampleRate = amp->choseBestSampleRate();

    if (!amp->openStream())
    {
        std::cerr << "[Error] Failed to open audio stream.\n";
        return;
    }

    if (!amp->startStream())
    {
        std::cerr << "[Error] Failed to start audio stream.\n";
        return;
    }

    DeviceInfo inDev = amp->getInputDevice();
    DeviceInfo outDev = amp->getOutputDevice();

    std::cout << "[Info] Audio stream started\n";
    if (inDev.index != 0)
        std::cout << "   Input : " << inDev.name << " (" << inDev.maxInputChannels << " channels)\n";
    if (outDev.index != 0)
        std::cout << "   Output: " << outDev.name << " (" << outDev.maxOutputChannels << " channels)\n";
    std::cout << "   Sample rate: " << amp->sampleRate << " Hz\n";
}

void CommandHandler::closeStream()
{
    amp->stopStream();
    std::cout << "[Info] Audio stream stopped.\n";
}

void CommandHandler::exitApp()
{
    std::cout << "Exiting Amply Digital Amplifier...\n";
    amp->stopStream();
    std::exit(0);
}

// ===================== Utility Functions =====================
void CommandHandler::showHelp()
{
    std::cout << "\nAvailable commands:\n";
    for (const auto &[cmd, _] : commands)
        std::cout << "  " << cmd << "\n";
}

void CommandHandler::clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "Welcome to Amply Digital Amplifier!\n";
    std::cout << "Type 'help' to see available commands.\n";
}

void CommandHandler::clearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
