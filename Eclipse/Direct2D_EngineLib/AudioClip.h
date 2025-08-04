#pragma once
#include <string>
#include <fmod.hpp>

// mp3, wav 등의 sound파일 리소스 asset
// Resource Manager를 통해서 생성해야합니다.

class AudioClip
{
private:
	FMOD::Sound* sound = nullptr;

public:
	AudioClip(const std::string& filePath);
	~AudioClip();

	FMOD::Sound* GetSound() const { return sound; }
	bool IsValid() const { return sound != nullptr; }
};

