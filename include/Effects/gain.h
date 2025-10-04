#pragma once

#include "effect.h"

class GainEffect : public Effect {
public:
    GainEffect(float gain = 1.0f) : gain(gain) {}

    float process(float inputSample) override {
        float output = inputSample * gain;
        if (output > 1.0f) output = 1.0f;
        if (output < -1.0f) output = -1.0f;
        return output;
    }

    void setGain(float g) { gain = g; }

private:
    float gain;
};
