#include "AudioSource.h"
#include "AudioSystem.h"


void AudioSource::OnEnable_Inner()
{
    AudioSystem::Get().Regist(this);
}

void AudioSource::OnDisable_Inner()
{
    Stop();
    AudioSystem::Get().Unregist(this);
}

void AudioSource::OnDestroy_Inner()
{
    Stop();
    AudioSystem::Get().Unregist(this);
}

void AudioSource::SetClip(AudioClip* newClip)
{
    clip = newClip;
}

void AudioSource::SetVolume(float volume)
{
    if (channel)
        channel->setVolume(volume);
}

float AudioSource::GetVolume()
{
    return channel->setVolume(volume);
}

void AudioSource::SetLoop(bool loop)
{
    isLoop = loop;
}

bool AudioSource::GetLoop()
{
    return isLoop;
}

void AudioSource::Play()
{
    if (!clip || !clip->IsValid()) return;

    // 재생중인 채널 정지
    if (channel)
    {
        bool isPlaying = false;
        channel->isPlaying(&isPlaying);
        if (isPlaying)
        {
            channel->stop();
        }
        channel = nullptr;
    }

    // loop
    FMOD_MODE mode = isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    clip->GetSound()->setMode(mode);

    // play
    if (system)
    {
        system->playSound(clip->GetSound(), nullptr, false, &channel);
    }
}

void AudioSource::PlayOneShot()
{
    if (!clip || !clip->IsValid()) return;

    // 재생중인 채널 정지
    if (channel)
    {
        bool isPlaying = false;
        channel->isPlaying(&isPlaying);
        if (isPlaying)
        {
            channel->stop();
        }
        channel = nullptr;
    }

    // loop
    FMOD_MODE mode = FMOD_LOOP_OFF;
    clip->GetSound()->setMode(mode);

    // play
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