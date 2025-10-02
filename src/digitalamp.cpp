#include "digitalamp.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

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

    currentApi_ = chooseBestApi();

    return true;
}

bool DigitalAmp::openStream(double sampleRate, unsigned long framesPerBuffer)
{
    if (!initialized_)
    {
        std::cerr << "PortAudio error: PortAudio is not initialized" << std::endl;
        return false;
    }

    // Verify that this format is supported
    PaError support = Pa_IsFormatSupported(&inputParams_, &outputParams_, sampleRate);
    if (support != paFormatIsSupported)
    {
        std::cerr << "Format not supported: " << Pa_GetErrorText(support) << std::endl;
        return false;
    }

    PaError err = Pa_OpenStream(&stream_,
                                &inputParams_,
                                &outputParams_,
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

std::unique_ptr<AvailableDevices> DigitalAmp::getAvailableDevices()
{
    auto result = std::make_unique<AvailableDevices>();

    const PaHostApiInfo *apiInfo = Pa_GetHostApiInfo(currentApi_);
    if (!apiInfo)
        return result;

    auto isVirtual = [](const std::string &n)
    {
        static const std::vector<std::string> blacklist = {
            "Monitor of", "dmix", "default", "null",
            "a52", "side", "rear", "center_lfe", "cards.pcm", "Loopback"};
        for (const auto &bad : blacklist)
        {
            if (n.find(bad) != std::string::npos)
                return true;
        }
        return false;
    };

    auto sanitizeName = [](const char *raw)
    {
        std::string s(raw ? raw : "");
        for (char &c : s)
        {
            if (!isprint(static_cast<unsigned char>(c)))
            {
                c = '?'; // replace unprintables
            }
        }
        return s;
    };

    for (int i = 0; i < apiInfo->deviceCount; i++)
    {
        PaDeviceIndex deviceIndex = Pa_HostApiDeviceIndexToDeviceIndex(currentApi_, i);
        const PaDeviceInfo *paDev = Pa_GetDeviceInfo(deviceIndex);
        if (!paDev)
            continue;

        std::string name = sanitizeName(paDev->name);
        if (isVirtual(name))
            continue; // skip ghost/virtual devices

        DeviceInfo d;
        d.index = deviceIndex;
        d.name = std::string(apiInfo->name) + ": " + name;
        d.maxInputChannels = paDev->maxInputChannels;
        d.maxOutputChannels = paDev->maxOutputChannels;

        if (d.maxInputChannels > 0)
            result->inputs.push_back(d);
        if (d.maxOutputChannels > 0)
            result->outputs.push_back(d);
    }

    return result;
}

double DigitalAmp::choseBestSampleRate()
{

    // Try from highest quality down
    static const double COMMON_SAMPLE_RATES[] = {
        192000.0, 96000.0, 88200.0,
        48000.0, 44100.0, 32000.0,
        22050.0, 16000.0, 8000.0};

    for (double rate : COMMON_SAMPLE_RATES)
    {
        if (Pa_IsFormatSupported(&inputParams_, &outputParams_, rate) == paFormatIsSupported)
        {
            return rate;
        }
    }

    return 44100.0;
}

bool DigitalAmp::createStreamParameters(PaDeviceIndex deviceIndex, int channelCount, PaSampleFormat sampleFormat, bool isInput)
{
    PaStreamParameters *params = isInput ? &inputParams_ : &outputParams_;
    const PaDeviceInfo *device = Pa_GetDeviceInfo(deviceIndex);

    if (device == NULL)
    {
        std::cerr << "PortAudio error: There is no device with index of " << deviceIndex << std::endl;
        return true;
    }

    params->device = deviceIndex;
    params->channelCount = channelCount;
    params->sampleFormat = sampleFormat;
    params->suggestedLatency = isInput ? device->defaultLowInputLatency : device->defaultLowOutputLatency;
    params->hostApiSpecificStreamInfo = nullptr;

    return false;
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

int DigitalAmp::chooseCommonChannelCount(const DeviceInfo &input, const DeviceInfo &output)
{
    int inCh = input.maxInputChannels;
    int outCh = output.maxOutputChannels;

    if (inCh <= 0 || outCh <= 0)
    {
        std::cerr << "One of the devices has no usable channels!\n";
        return 0;
    }

    int common = std::min(inCh, outCh);
    if (common < 1)
        common = 1; // must be >= 1

    return common;
}