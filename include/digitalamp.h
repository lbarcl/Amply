#pragma once
#include <portaudio.h>
#include <memory>
#include <vector>
#include "utils.h"

class DigitalAmp {
public:
    // ===================== Constructor / Destructor =====================
    DigitalAmp();
    ~DigitalAmp();
    
    // ===================== Initialization / Termination =====================
    bool initialize();
    
    // ===================== Stream Management =====================
    bool openStream();
    bool openStream(double sampleRate, unsigned long framesPerBuffer);
    bool createStreamParameters(PaDeviceIndex deviceIndex, int channelCount, PaSampleFormat sampleFormat, bool isInput);
    bool startStream();
    void stopStream();
    
    // ===================== Sample Rate Handling =====================
    std::vector<double> getSupportedSampleRates(const PaStreamParameters* inputParams, const PaStreamParameters* outputParams);
    std::vector<double> getSupportedSampleRates();
    double choseBestSampleRate();
    
    // ===================== Channel Handling =====================
    int chooseCommonChannelCount(const DeviceInfo &input, const DeviceInfo &output);
    int chooseCommonChannelCount(int inputCount, int outputCount);
    bool chooseCommonChannelCount();
    
    // ===================== Device Handling =====================
    DeviceInfo getDefaultDevice(bool isInput = true);
    DeviceInfo getInputDevice();
    DeviceInfo getOutputDevice();
    std::unique_ptr<AvailableDevices> getAvailableDevices();
    
    // ===================== Public Members =====================
    double sampleRate;

private:
    // ===================== Audio Processing =====================
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData);
    int processAudio(const float* input, float* output, unsigned long frameCount);

    // ===================== Internal State =====================
    PaHostApiIndex currentApi_;
    PaStream* stream_;
    PaStreamParameters inputParams_;
    PaStreamParameters outputParams_;
    bool initialized_;
    bool running_;
};
