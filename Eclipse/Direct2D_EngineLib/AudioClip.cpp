#include "AudioClip.h"

AudioClip::AudioClip(FMOD::System* system, const std::string& filepath)
{
    if (system)
    {
        FMOD_RESULT result = system->createSound(filepath.c_str(), FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK) sound = nullptr;  
    }
}

AudioClip::~AudioClip()
{
    if (sound)
    {
        sound->release();
        sound = nullptr;
    }
}