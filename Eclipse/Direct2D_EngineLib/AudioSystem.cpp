#include "AudioSystem.h"
#include "AudioSource.h"
#include <Windows.h>

// component 등록
void AudioSystem::Regist(AudioSource* component)
{
	components.push_back(component);
}

// component 등록 해제
void AudioSystem::Unregist(AudioSource* component)
{
	// delete
	for (auto it = components.begin(); it != components.end(); ++it) {
		if (*it == component) {
			components.erase(it);
			return;
		}
	}
}

void AudioSystem::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		OutputDebugStringA("[Woo Engine] FMOD System 생성 실패\n");
	}

	result = system->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		OutputDebugStringA("[Woo Engine] FMOD System 초기화 실패\n");
	}
}

void AudioSystem::Update()
{
	// system update
	if (system)
		system->update();
}

void AudioSystem::UnInit()
{
	if (system)
	{
		system->close();
		system->release();
		system = nullptr;
	}
}