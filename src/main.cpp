#include "digitalamp.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void printUsage() {
    std::cout << "Amply - Digital Guitar Amplifier\n";
    std::cout << "Usage: amply [options]\n";
    std::cout << "Commands while running:\n";
    std::cout << "  g <value>  - Set gain (0.0 - 10.0)\n";
    std::cout << "  d <value>  - Set distortion (0.0 - 1.0)\n";
    std::cout << "  q          - Quit\n";
    std::cout << "  h          - Show this help\n";
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "Starting Amply Digital Amplifier...\n";
    
    DigitalAmp amp;
    
    // Initialize the amplifier
    if (!amp.initialize()) {
        std::cerr << "Failed to initialize amplifier" << std::endl;
        return 1;
    }
    
    // Start audio processing
    if (!amp.start()) {
        std::cerr << "Failed to start audio stream" << std::endl;
        return 1;
    }
    
    std::cout << "Amplifier started successfully!\n";
    std::cout << "Connect your guitar to the default input device.\n";
    printUsage();
    
    // Interactive command loop
    std::string input;
    while (true) {
        std::cout << "amply> ";
        std::getline(std::cin, input);
        
        if (input.empty()) continue;
        
        char command = input[0];
        switch (command) {
            case 'q':
            case 'Q':
                std::cout << "Shutting down amplifier...\n";
                amp.stop();
                return 0;
                
            case 'h':
            case 'H':
                printUsage();
                break;
                
            case 'g':
            case 'G':
                if (input.length() > 2) {
                    try {
                        float gain = std::stof(input.substr(2));
                        amp.setGain(gain);
                        std::cout << "Gain set to: " << gain << std::endl;
                    } catch (const std::exception& e) {
                        std::cout << "Invalid gain value. Use format: g 2.5\n";
                    }
                } else {
                    std::cout << "Usage: g <value> (0.0 - 10.0)\n";
                }
                break;
                
            case 'd':
            case 'D':
                if (input.length() > 2) {
                    try {
                        float distortion = std::stof(input.substr(2));
                        amp.setDistortion(distortion);
                        std::cout << "Distortion set to: " << distortion << std::endl;
                    } catch (const std::exception& e) {
                        std::cout << "Invalid distortion value. Use format: d 0.5\n";
                    }
                } else {
                    std::cout << "Usage: d <value> (0.0 - 1.0)\n";
                }
                break;
                
            default:
                std::cout << "Unknown command. Type 'h' for help.\n";
                break;
        }
    }
    
    return 0;
}