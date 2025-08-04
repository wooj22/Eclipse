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
    return volume;
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

    // ������� ä�� ����
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
        
        // group
        if (channel && outputChannel)
            channel->setChannelGroup(outputChannel);
    }
}

void AudioSource::PlayOneShot()
{
    if (!clip || !clip->IsValid()) return;

    // ������� ä�� ����
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
   
        // group
        if (channel && outputChannel)
            channel->setChannelGroup(outputChannel);
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

void AudioSource::SetChannelGroup(FMOD::ChannelGroup* channelGroup)
{
    outputChannel = channelGroup;
}

FMOD::ChannelGroup* AudioSource::GetChannelGroup()
{
    return outputChannel;
}