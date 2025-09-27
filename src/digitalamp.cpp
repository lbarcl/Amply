#include "digitalamp.h"
#include <iostream>
#include <cmath>
#include <algorithm>

DigitalAmp::DigitalAmp()
    : stream_(nullptr), initialized_(false), running_(false)
{
}

DigitalAmp::~DigitalAmp()
{
    stopStream();
    if (initialized_)
    {
        Pa_Terminate();
    }
}

bool DigitalAmp::initialize()
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    initialized_ = true;

    return true;
}

bool DigitalAmp::openStream(PaStreamParameters inputParameters, PaStreamParameters outputParameters, double sampleRate, unsigned long framesPerBuffer)
{

    if (!initialized_)
    {
        std::cerr << "PortAudio error: PortAudio is not initialized" << std::endl;
        return false;
    }

    PaError err = Pa_OpenStream(&stream_,
                        &inputParameters,
                        &outputParameters,
                        sampleRate,
                        framesPerBuffer,
                        paClipOff,
                        audioCallback,
                        this);

    if (err != paNoError)
    {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    return true;
}

bool DigitalAmp::startStream()
{
    if (!initialized_ || !stream_)
    {
        return false;
    }

    PaError err = Pa_StartStream(stream_);
    if (err != paNoError)
    {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    running_ = true;
    return true;
}

void DigitalAmp::stopStream()
{
    if (stream_)
    {
        Pa_CloseStream(stream_);
        stream_ = nullptr;
    }
    running_ = false;
}

PaStreamParameters* DigitalAmp::createStreamParameters(PaDeviceIndex deviceIndex, int channelCount, PaSampleFormat sampleFormat, bool isInput = true)
{   
    PaStreamParameters params;
    const PaDeviceInfo* device = Pa_GetDeviceInfo(deviceIndex);
    
    if (device == NULL) {
        std::cerr << "PortAudio error: There is no device with index of " << deviceIndex  << std::endl;
        return NULL;
    }

    params.device = deviceIndex;
    params.channelCount = channelCount;
    params.sampleFormat = sampleFormat;
    params.suggestedLatency = isInput ? device->defaultLowInputLatency : device->defaultLowOutputLatency;
    params.hostApiSpecificStreamInfo = nullptr;

    return &params;
}

int DigitalAmp::audioCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData)
{
    DigitalAmp *amp = static_cast<DigitalAmp *>(userData);
    const float *input = static_cast<const float *>(inputBuffer);
    float *output = static_cast<float *>(outputBuffer);

    return amp->processAudio(input, output, framesPerBuffer);
}

int DigitalAmp::processAudio(const float *input, float *output, unsigned long frameCount)
{
    for (unsigned long i = 0; i < frameCount; i++)
    {
        float sample = input[i];
        // Clip to prevent overflow
        sample = std::max(-1.0f, std::min(1.0f, sample));

        output[i] = sample;
    }

    return paContinue;
}