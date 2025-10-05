#include "utils.h"
#include <iostream>
#include <string>
#include <vector>

// ===================== Host API Selection =====================

// Utility: pick best host API based on OS & fallback priority list
PaHostApiIndex chooseBestApi() 
{

#ifdef _WIN32
    std::vector<PaHostApiTypeId> priority = {
        paASIO, paWASAPI, paWDMKS, paDirectSound, paMME
    };
#elif __APPLE__
    std::vector<PaHostApiTypeId> priority = { paCoreAudio };
#else // Linux & others
    std::vector<PaHostApiTypeId> priority = {
        paPulseAudio, paALSA, paJACK, paOSS
    };
#endif

    int numApis = Pa_GetHostApiCount();
    if (numApis < 0) 
        return paHostApiNotFound;

    // Try APIs in order of priority
    for (const PaHostApiTypeId &pref : priority) 
    {
        for (int i = 0; i < numApis; i++) 
        {
            const PaHostApiInfo* apiInfo = Pa_GetHostApiInfo(i);
            if (apiInfo && apiInfo->type == pref) 
            {   
                return i;
            }
        }
    }

    // Fallback to default API
    return Pa_GetDefaultHostApi();
}
