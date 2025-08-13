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
    float windTimer = 0.0f;
    float owlInterval = 40.0f;
    float wolfInterval = 60.0f;
    float windInterval = 15.0f;
    float windDuration = 20.0f;
    bool isWindPlaying = false;

public:
    AmbientSoundManager();
    virtual ~AmbientSoundManager();

    void Awake() override;
    void Update() override;
    
    void SetOwlInterval(float interval) { owlInterval = interval; }
    void SetWolfInterval(float interval) { wolfInterval = interval; }
    
    // Getter methods
    float GetOwlInterval() const { return owlInterval; }
    float GetWolfInterval() const { return wolfInterval; }
    float GetOwlTimer() const { return owlTimer; }
    float GetWolfTimer() const { return wolfTimer; }
    
    void PlayWindSound();
    void StopWindSound();
    void PlayRandomOwlSound();
    void PlayRandomWolfSound();
};