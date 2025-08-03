#pragma once
#include <fmod.hpp>
#include "AudioClip.h"
#include "AudioSystem.h"
#include "Component.h"

/* [AudioSource Component]
* 
*/

class AudioSource : public Component
{
private:
    FMOD::System* system = nullptr;             // fmod sound system
    AudioClip* clip = nullptr;                  // �� audiosource�� ����� audioClip
    FMOD::Channel* channel = nullptr;           // sound�� ��µ� channel

public:
    // component cycle
    AudioSource(FMOD::System* sys);
    ~AudioSource() override;

    void OnEnable_Inner() override final;
    void OnDisable_Inner() override final;
    void OnDestroy_Inner() override final;

public:
    // functions
    void SetClip(AudioClip* newClip);
    void Play(bool loop = false);
    void Stop();
    bool IsPlaying() const;
    void SetVolume(float volume);
};

