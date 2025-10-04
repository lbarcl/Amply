#pragma once
#include <unordered_map>
#include <functional>
#include <string>

class DigitalAmp; // forward declaration

class CommandHandler {
public:
    explicit CommandHandler(DigitalAmp* amp);
    void run();

private:
    DigitalAmp* amp;

    // Map from command strings to functions
    std::unordered_map<std::string, std::function<void()>> commands;

    // Command handlers
    void chooseInput();
    void chooseOutput();
    void chooseSampleRate();
    void startStream();
    void closeStream();
    void exitApp();
    void showHelp();
    void clearConsole();

    // Utility
    void clearInputBuffer();
};
