#include "AudioSource.h"

AudioSource::AudioSource(FMOD::System* sys)
{
    system = sys;
}

AudioSource::~AudioSource()
{
    Stop();
}

void AudioSource::OnEnable_Inner()
{
    AudioSystem::Get().Regist(this);
}

void AudioSource::OnDisable_Inner()
{
    AudioSystem::Get().Unregist(this);
}

void AudioSource::OnDestroy_Inner()
{
    AudioSystem::Get().Unregist(this);
}

void AudioSource::SetClip(AudioClip* newClip)
{
    clip = newClip;
}

void AudioSource::Play(bool loop)
{
    if (!clip || !clip->IsValid()) return;

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    clip->GetSound()->setMode(mode);

    if (system)
    {
        system->playSound(clip->GetSound(), nullptr, false, &channel);
    }
}

void AudioSource::Stop()
{
    if (channel)
    {
        channel->stop();
        channel = nullptr;
    }
}

bool AudioSource::IsPlaying() const
{
    bool playing = false;
    if (channel)
        channel->isPlaying(&playing);
    return playing;
}

void AudioSource::SetVolume(float volume)
{
    if (channel)
        channel->setVolume(volume);
}
