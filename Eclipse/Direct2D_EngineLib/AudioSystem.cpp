#include "AudioSystem.h"
#include "AudioSource.h"

// component 등록
void AudioSystem::Regist(AudioSource* component)
{
	pending_components.push_back(component);
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

	// pending delete
	for (auto it = pending_components.begin(); it != pending_components.end(); ++it) {
		if (*it == component) {
			pending_components.erase(it);
			return;
		}
	}
}

// component system
void AudioSystem::Update()
{
	// pending_components push
	for (AudioSource* component : pending_components)
	{
		components.push_back(component);
	}
	pending_components.clear();


	// update
	for (AudioSource* component : components)
	{
		//component->Update();
	}
}
