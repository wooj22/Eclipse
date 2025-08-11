#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/AudioSystem.h"
#include <vector>
#include <string>

class AudioSource;

class AmbientSoundManager : public GameObject
{
private:
    AudioSource* windSound;
    std::vector<AudioSource*> owlSounds;
    std::vector<AudioSource*> wolfSounds;
    
    float owlTimer = 0.0f;
    float wolfTimer = 0.0f;
    float owlInterval = 10.0f;
    float wolfInterval = 15.0f;

public:
    AmbientSoundManager();
    virtual ~AmbientSoundManager();

    void Awake() override;
    void Update() override;
    
    void SetOwlInterval(float interval) { owlInterval = interval; }
    void SetWolfInterval(float interval) { wolfInterval = interval; }
    
    void PlayWindSound();
    void StopWindSound();
    void PlayRandomOwlSound();
    void PlayRandomWolfSound();
};