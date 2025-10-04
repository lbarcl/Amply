#include "commandhandler.h"
#include "digitalamp.h"

int main() { 
    DigitalAmp amp;
    amp.initialize();

    CommandHandler cmd(&amp);
    cmd.run();

    return 0;
}