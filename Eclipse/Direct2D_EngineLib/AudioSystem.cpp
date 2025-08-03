#include "AudioSystem.h"
#include "AudioSource.h"
#include <Windows.h>

// component ���
void AudioSystem::Regist(AudioSource* component)
{
	pending_components.push_back(component);
}

// component ��� ����
void AudioSystem::Unregist(AudioSource* component)
{
	// delete
	for (auto it = components.begin(); it != components.end(); ++it) {
		if (*it == component) {
			components.erase(it);
			return;
		}
	}

	// pending delete
	for (auto it = pending_components.begin(); it != pending_components.end(); ++it) {
		if (*it == component) {
			pending_components.erase(it);
			return;
		}
	}
}

void AudioSystem::Init()
{
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
}

void AudioSystem::Update()
{
	// pending_components push
	for (AudioSource* component : pending_components)
	{
		components.push_back(component);
	}
	pending_components.clear();

	//system update
	if (system)
		system->update();

	// update
	for (AudioSource* component : components)
	{
		//component->Update();
	}
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