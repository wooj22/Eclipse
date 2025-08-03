#pragma once
#include <string>
#include <fmod.hpp>

// mp3, wav 殿狼 sound颇老 府家胶 asset
// TODO :: Resource Manager 包府

class AudioClip
{
private:
	FMOD::Sound* sound = nullptr;

public:
	AudioClip(FMOD::System* system, const std::string& filePath);
	~AudioClip();

	FMOD::Sound* GetSound() const { return sound; }
	bool IsValid() const { return sound != nullptr; }
};

