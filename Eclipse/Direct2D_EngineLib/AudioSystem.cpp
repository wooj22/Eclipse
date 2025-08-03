#include "AudioSystem.h"
#include "AudioSource.h"
#include <Windows.h>

// component ���
void AudioSystem::Regist(AudioSource* source)
{
	sources.push_back(source);
}

// component ��� ����
void AudioSystem::Unregist(AudioSource* source)
{
	// delete
	for (auto it = sources.begin(); it != sources.end(); ++it) {
		if (*it == source) {
			sources.erase(it);
			return;
		}
	}
}

void AudioSystem::Init()
{
	// fmod system
	FMOD_RESULT result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		OutputDebugStringA("[Woo Engine] FMOD System ���� ����\n");
	}

	result = system->init(512, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		OutputDebugStringA("[Woo Engine] FMOD System �ʱ�ȭ ����\n");
	}

	// channel grouping
	system->createChannelGroup("Master", &masterGroup);
	system->createChannelGroup("BGM", &bgmGroup);
	system->createChannelGroup("SFX", &sfxGroup);

	masterGroup->addGroup(bgmGroup);
	masterGroup->addGroup(sfxGroup);
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

void AudioSystem::SetMasterVolume(float volume)
{
	if (masterGroup)
		masterGroup->setVolume(volume);
}

void AudioSystem::SetBGMVolume(float volume)
{
	if (bgmGroup)
		bgmGroup->setVolume(volume);
}

void AudioSystem::SetSFXVolume(float volume)
{
	if (sfxGroup)
		sfxGroup->setVolume(volume);
}