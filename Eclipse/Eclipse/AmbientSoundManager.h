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
    std::vector<AudioSource*> animalSounds;  // Combined owl and wolf sounds
    
    float animalSoundTimer = 0.0f;
    float animalSoundInterval = 8.0f;  // 8 seconds interval
    float windTimer = 0.0f;
    float windInterval = 15.0f;
    float windDuration = 20.0f;
    bool isWindPlaying = false;

public:
    AmbientSoundManager();
    virtual ~AmbientSoundManager();

    void Awake() override;
    void Update() override;
    
    void SetAnimalSoundInterval(float interval) { animalSoundInterval = interval; }
    
    // Getter methods
    float GetAnimalSoundInterval() const { return animalSoundInterval; }
    float GetAnimalSoundTimer() const { return animalSoundTimer; }
    
    void PlayWindSound();
    void StopWindSound();
    void PlayRandomAnimalSound();
};