#pragma once
#include <fmod.hpp>
#include "AudioClip.h"
#include "AudioSystem.h"
#include "Component.h"
#include <iostream>

/* [AudioSource Component]
* 
*/

class AudioSource : public Component
{
private:
    FMOD::System* system = nullptr;             // fmod sound system
    shared_ptr<AudioClip> clip = nullptr;       // 이 audiosource가 재생할 audioClip
    FMOD::Channel* channel = nullptr;           // sound가 출력될 channel

    // channel output group
    FMOD::ChannelGroup* outputChannel = nullptr;

    // audoi source data
    float volume = 1.0;
    bool isLoop = false;

public:
    // component cycle
    AudioSource() { system = AudioSystem::Get().GetSystem(); }
    ~AudioSource() override = default;

    void OnEnable_Inner() override final;
    void OnDisable_Inner() override final;
    void OnDestroy_Inner() override final;

public:
    // functions
    void SetClip(shared_ptr<AudioClip> newClip);
    void SetVolume(float volume);
    float GetVolume();
    void SetLoop(bool loop);
    bool GetLoop();
    void Play();
    void PlayOneShot();
    void Stop();
    bool IsPlaying() const;

    // group
    void SetChannelGroup(FMOD::ChannelGroup* channelGroup);
    FMOD::ChannelGroup* GetChannelGroup();
};

