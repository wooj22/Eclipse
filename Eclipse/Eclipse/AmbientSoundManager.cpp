#include "AmbientSoundManager.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/Time.h"
#include <cstdlib>
#include <ctime>

AmbientSoundManager::AmbientSoundManager() : GameObject("AmbientSoundManager", "AmbientSound")
{
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Wind sound
    windSound = AddComponent<AudioSource>();
    windSound->SetChannelGroup(AudioSystem::Get().GetAMBGroup());
    auto windClip = ResourceManager::Get().CreateAudioClip("../Resource/Aron/AMB/s_Wind.wav");
    windSound->SetClip(windClip);
    windSound->SetLoop(false);
    windSound->SetVolume(0.8f);
    
    // Owl sounds
    for (int i = 1; i <= 3; i++)
    {
        AudioSource* owlSource = AddComponent<AudioSource>();
        owlSource->SetChannelGroup(AudioSystem::Get().GetAMBGroup());
        
        std::string filename = "../Resource/Aron/AMB/s_Owl" + std::to_string(i) + ".wav";
        
        auto owlClip = ResourceManager::Get().CreateAudioClip(filename);
        owlSource->SetClip(owlClip);
        owlSource->SetLoop(false);
        owlSource->SetVolume(0.9f);
        owlSounds.push_back(owlSource);
        animalSounds.push_back(owlSource);  // Add to combined list
    }
    
    // Wolf sounds
    for (int i = 1; i <= 2; i++)
    {
        AudioSource* wolfSource = AddComponent<AudioSource>();
        wolfSource->SetChannelGroup(AudioSystem::Get().GetAMBGroup());
        
        std::string filename = "../Resource/Aron/AMB/s_Wolf" + std::to_string(i) + ".wav";
        
        auto wolfClip = ResourceManager::Get().CreateAudioClip(filename);
        wolfSource->SetClip(wolfClip);
        wolfSource->SetLoop(false);
        wolfSource->SetVolume(0.8f);
        wolfSounds.push_back(wolfSource);
        animalSounds.push_back(wolfSource);  // Add to combined list
    }
}

AmbientSoundManager::~AmbientSoundManager()
{
}

void AmbientSoundManager::Awake()
{
    GameObject::Awake();
}

void AmbientSoundManager::Update()
{
    GameObject::Update();
    
    float deltaTime = Time::GetDeltaTime();
    
    // Update wind timer
    windTimer += deltaTime;
    if (windTimer >= windInterval)
    {
        PlayWindSound();
        windTimer = 0.0f;
        // Randomize next interval (play every 30-50 seconds)
        windInterval = 30.0f + (rand() % 20);
    }
    
    // Update animal sound timer (owl and wolf combined)
    animalSoundTimer += deltaTime;
    if (animalSoundTimer >= animalSoundInterval)
    {
        PlayRandomAnimalSound();
        animalSoundTimer = 0.0f;
        // Fixed 8 second interval
    }
}

void AmbientSoundManager::PlayWindSound()
{
    if (windSound)
    {
        windSound->Play();
    }
}

void AmbientSoundManager::StopWindSound()
{
    if (windSound)
    {
        windSound->Stop();
    }
}

void AmbientSoundManager::PlayRandomAnimalSound()
{
    if (!animalSounds.empty())
    {
        int randomIndex = rand() % animalSounds.size();
        if (animalSounds[randomIndex])
        {
            animalSounds[randomIndex]->Play();
        }
    }
}