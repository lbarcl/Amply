#pragma once
#include <portaudio.h>
#include <memory>

#include "utils.h"

class DigitalAmp {
public:
    DigitalAmp();
    ~DigitalAmp();
    
    bool initialize();
    bool openStream(double sampleRate, unsigned long framesPerBuffer);
    bool createStreamParameters(PaDeviceIndex deviceIndex, int channelCount, PaSampleFormat sampleFormat, bool isInput);
    
    bool startStream();
    void stopStream();
    
    double choseBestSampleRate();    
    int chooseCommonChannelCount(const DeviceInfo &input, const DeviceInfo &output);

    std::unique_ptr<AvailableDevices> getAvailableDevices();
private:
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);
    
    int processAudio(const float* input, float* output, unsigned long frameCount);


    PaHostApiIndex currentApi_;
    PaStream* stream_;
    PaStreamParameters inputParams_;
    PaStreamParameters outputParams_;
    bool initialized_;
    bool running_;
};
