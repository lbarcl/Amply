#include "Effects/gain.h"
#include "digitalamp.h"

int main() { 
    std::shared_ptr<GainEffect> gain = std::make_shared<GainEffect>(2.0f);

    DigitalAmp amp;
    amp.initialize();
    amp.effects.push_back(gain);

    return 0;
}