#include "AudioClip.h"
#include "AudioSystem.h"

AudioClip::AudioClip(const std::string& filepath)
{
    // filepath의 sound 파일 생성
    FMOD::System* system = AudioSystem::Get().GetSystem();
    if (system)
    {
        FMOD_RESULT result = system->createSound(filepath.c_str(), FMOD_DEFAULT, nullptr, &sound);
        if (result == FMOD_OK && sound)
        {
            unsigned int lengthMS = 0;
            sound->getLength(&lengthMS, FMOD_TIMEUNIT_MS);
            length = lengthMS / 1000.0f;
        }
        else
        {
            sound = nullptr;
        }
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