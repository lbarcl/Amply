#pragma once

class Effect {
public:
    virtual ~Effect() = default;

    // Process a single sample
    virtual float process(float inputSample) = 0;
};