#include "AudioClip.h"
#include "AudioSystem.h"

AudioClip::AudioClip(const std::string& filepath)
{
    // filepath�� sound ���� ����
    FMOD::System* system = AudioSystem::Get().GetSystem();
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