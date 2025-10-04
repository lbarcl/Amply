#include "commandhandler.h"
#include "digitalamp.h"

int main() { 
    std::shared_ptr<GainEffect> gain = std::make_shared<GainEffect>(2.0f);

    DigitalAmp amp;
    amp.initialize();
    amp.effects.push_back(gain);

    CommandHandler cmd(&amp);
    cmd.gainEffect = gain;
    cmd.run();

    return 0;
}