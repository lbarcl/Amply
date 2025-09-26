#pragma once

#include <portaudio.h>
#include <memory>

class DigitalAmp {
public:
    DigitalAmp();
    ~DigitalAmp();
    
    bool initialize();
    bool start();
    void stop();
    
    void setGain(float gain);
    void setDistortion(float distortion);
    
private:
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);
    
    int processAudio(const float* input, float* output, unsigned long frameCount);
    float applyDistortion(float sample);
    
    PaStream* stream_;
    float gain_;
    float distortion_;
    bool initialized_;
    bool running_;
};