#pragma once

#include <portaudio.h>
#include <memory>
#include <vector>
#include <string>

// A safe copy of device information
struct DeviceInfo {
    int index;              // PortAudio device index
    std::string name;       // Device name
    int maxInputChannels;   // >0 means input
    int maxOutputChannels;  // >0 means output
};

// A container of available devices
struct AvailableDevices {
    public:
    std::vector<DeviceInfo> inputs;
    std::vector<DeviceInfo> outputs;
};

class DigitalAmp {
public:
    DigitalAmp();
    ~DigitalAmp();
    
    bool initialize();
    bool openStream(PaStreamParameters inputParameters, PaStreamParameters outputParameters, double sampleRate, unsigned long framesPerBuffer);
    bool startStream();
    void stopStream();

    std::unique_ptr<AvailableDevices> getAvailableDevices();
    PaStreamParameters* createStreamParameters(PaDeviceIndex deviceIndex, int channelCount, PaSampleFormat sampleFormat, bool isInput);
    
private:
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);
    
    int processAudio(const float* input, float* output, unsigned long frameCount);
    
    PaHostApiIndex currentApi;
    PaStream* stream_;
    bool initialized_;
    bool running_;
};

PaHostApiIndex chooseBestApi();
