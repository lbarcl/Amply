#pragma once

#include <portaudio.h>
#include <memory>

class DigitalAmp {
public:
    DigitalAmp();
    ~DigitalAmp();
    
    bool initialize();
    bool openStream(PaStreamParameters inputParameters, PaStreamParameters outputParameters, double sampleRate, unsigned long framesPerBuffer);
    bool startStream();
    void stopStream();

    PaStreamParameters* createStreamParameters(PaDeviceIndex deviceIndex, int channelCount, PaSampleFormat sampleFormat, bool isInput);
    
private:
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);
    
    int processAudio(const float* input, float* output, unsigned long frameCount);
    
    PaStream* stream_;
    bool initialized_;
    bool running_;
};