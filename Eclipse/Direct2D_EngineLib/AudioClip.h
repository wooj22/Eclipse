#pragma once
#include <string>
#include <fmod.hpp>

// mp3, wav ���� sound���� ���ҽ� asset
// Resource Manager�� ���ؼ� �����ؾ��մϴ�.

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

