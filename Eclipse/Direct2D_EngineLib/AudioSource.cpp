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

void AudioSource::SetClip(shared_ptr<AudioClip> newClip)
{
    clip = newClip;
}

void AudioSource::SetVolume(float volume)
{
    this->volume = volume;
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

float AudioSource::GetCurrentClipLenght()
{
    return clip->GetLenght();
}

float AudioSource::GetCurrentPlayTime()
{
    unsigned int posMS = 0;
    FMOD_RESULT result = channel->getPosition(&posMS, FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK) return 0.0f;

    return posMS / 1000.0f;
}

float AudioSource::GetPlaybackProgress()
{
    float current = GetCurrentPlayTime();
    float total = GetCurrentClipLenght();

    if (total <= 0.0f) return 0.0f;
    return current / total; 
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
        
        if (channel) channel->setVolume(volume);
        if (outputChannel) channel->setChannelGroup(outputChannel);
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

        if (channel) channel->setVolume(volume);
        if (outputChannel) channel->setChannelGroup(outputChannel);
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