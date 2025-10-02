#include "utils.h"

// Utility: pick best host API based on OS & fallback list
PaHostApiIndex chooseBestApi() {
#ifdef _WIN32
    std::vector<std::string> priority = {
        "Windows WASAPI", "Windows WDM-KS", "Windows DirectSound", "MME"
    };
#elif __APPLE__
    std::vector<std::string> priority = { "Core Audio" };
#else // Linux & others
    std::vector<std::string> priority = {
        "PulseAudio", "ALSA", "JACK Audio Connection Kit", "OSS"
    };
#endif

    int numApis = Pa_GetHostApiCount();
    if (numApis < 0) return paHostApiNotFound;

    // Try APIs in order of priority
    for (auto &pref : priority) {
        for (int i = 0; i < numApis; i++) {
            const PaHostApiInfo* apiInfo = Pa_GetHostApiInfo(i);
            if (std::string(apiInfo->name).find(pref) != std::string::npos) {
                return i;
            }
        }
    }

    // fallback to default API
    return Pa_GetDefaultHostApi();
}