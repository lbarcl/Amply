#include "digitalamp.h"
#include <iostream>
#include <cmath>
#include <algorithm>

DigitalAmp::DigitalAmp() 
    : stream_(nullptr), gain_(1.0f), distortion_(0.0f), initialized_(false), running_(false) {
}

DigitalAmp::~DigitalAmp() {
    stop();
    if (initialized_) {
        Pa_Terminate();
    }
}

bool DigitalAmp::initialize() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    initialized_ = true;
    
    // Setup audio stream parameters
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        std::cerr << "Error: No default input device." << std::endl;
        return false;
    }
    
    inputParameters.channelCount = 1;       // mono input
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Error: No default output device." << std::endl;
        return false;
    }
    
    outputParameters.channelCount = 1;      // mono output
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    
    err = Pa_OpenStream(&stream_,
                        &inputParameters,
                        &outputParameters,
                        44100,  // sample rate
                        256,    // frames per buffer
                        paClipOff,
                        audioCallback,
                        this);
    
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    return true;
}

bool DigitalAmp::start() {
    if (!initialized_ || !stream_) {
        return false;
    }
    
    PaError err = Pa_StartStream(stream_);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    running_ = true;
    return true;
}

void DigitalAmp::stop() {
    if (stream_) {
        Pa_CloseStream(stream_);
        stream_ = nullptr;
    }
    running_ = false;
}

void DigitalAmp::setGain(float gain) {
    gain_ = std::max(0.0f, std::min(10.0f, gain));
}

void DigitalAmp::setDistortion(float distortion) {
    distortion_ = std::max(0.0f, std::min(1.0f, distortion));
}

int DigitalAmp::audioCallback(const void* inputBuffer, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData) {
    DigitalAmp* amp = static_cast<DigitalAmp*>(userData);
    const float* input = static_cast<const float*>(inputBuffer);
    float* output = static_cast<float*>(outputBuffer);
    
    return amp->processAudio(input, output, framesPerBuffer);
}

int DigitalAmp::processAudio(const float* input, float* output, unsigned long frameCount) {
    for (unsigned long i = 0; i < frameCount; i++) {
        float sample = input[i];
        
        // Apply gain
        sample *= gain_;
        
        // Apply distortion if enabled
        if (distortion_ > 0.0f) {
            sample = applyDistortion(sample);
        }
        
        // Clip to prevent overflow
        sample = std::max(-1.0f, std::min(1.0f, sample));
        
        output[i] = sample;
    }
    
    return paContinue;
}

float DigitalAmp::applyDistortion(float sample) {
    // Simple tanh distortion
    float distorted = std::tanh(sample * (1.0f + distortion_ * 10.0f));
    return sample * (1.0f - distortion_) + distorted * distortion_;
}