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
    windSound->SetLoop(true);
    windSound->SetVolume(0.8f);
    
    // Owl sounds
    for (int i = 1; i <= 3; i++)
    {
        AudioSource* owlSource = AddComponent<AudioSource>();
        owlSource->SetChannelGroup(AudioSystem::Get().GetAMBGroup());
        
        std::string filename = "../Resource/Aron/AMB/s_Owl";
        if (i > 1) filename += std::to_string(i);
        filename += ".wav";
        
        auto owlClip = ResourceManager::Get().CreateAudioClip(filename);
        owlSource->SetClip(owlClip);
        owlSource->SetLoop(false);
        owlSource->SetVolume(0.9f);
        owlSounds.push_back(owlSource);
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
    
    // Update owl timer
    owlTimer += deltaTime;
    if (owlTimer >= owlInterval)
    {
        PlayRandomOwlSound();
        owlTimer = 0.0f;
        // Randomize next interval
        owlInterval = 8.0f + (rand() % 8);
    }
    
    // Update wolf timer
    wolfTimer += deltaTime;
    if (wolfTimer >= wolfInterval)
    {
        PlayRandomWolfSound();
        wolfTimer = 0.0f;
        // Randomize next interval
        wolfInterval = 12.0f + (rand() % 10);
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

void AmbientSoundManager::PlayRandomOwlSound()
{
    if (!owlSounds.empty())
    {
        int randomIndex = rand() % owlSounds.size();
        if (owlSounds[randomIndex])
        {
            owlSounds[randomIndex]->Play();
        }
    }
}

void AmbientSoundManager::PlayRandomWolfSound()
{
    if (!wolfSounds.empty())
    {
        int randomIndex = rand() % wolfSounds.size();
        if (wolfSounds[randomIndex])
        {
            wolfSounds[randomIndex]->Play();
        }
    }
}