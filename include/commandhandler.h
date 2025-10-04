#pragma once
#include <unordered_map>
#include <functional>
#include <string>

class DigitalAmp; // Forward Declaration

class CommandHandler {
public:
    // ===================== Constructor =====================
    explicit CommandHandler(DigitalAmp* amp);

    // ===================== Main Loop =====================
    void run();

private:
    DigitalAmp* amp;

    std::unordered_map<std::string, std::function<void()>> commands;

    // ===================== Command Handlers =====================
    void chooseInput();
    void chooseOutput();
    void chooseSampleRate();
    void startStream();
    void closeStream();
    void exitApp();
    void showHelp();
    void clearConsole();

    // ===================== Utility =====================
    void clearInputBuffer();
};
