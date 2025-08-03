#pragma once
#include <fmod.hpp>
#include <vector>
#include "Singleton.h"
using namespace std;

/* AudioSystem�� ����
* 1. FMOD �ý��� �ʱ�ȭ �� ������Ʈ
*/

class AudioSource;
class AudioSystem : public Singleton<AudioSystem>
{
private:
	// FMOD system
	FMOD::System* system = nullptr;			

	// channel group
	FMOD::ChannelGroup* masterGroup = nullptr;
	FMOD::ChannelGroup* bgmGroup = nullptr;
	FMOD::ChannelGroup* sfxGroup = nullptr;

private:
	vector<AudioSource*> sources;

public:
	// componenet
	void Regist(AudioSource* source);
	void Unregist(AudioSource* source);

	// component system
	void Init();
	void Update();
	void UnInit();

public:
	FMOD::System* GetSystem() const { return system; }

	FMOD::ChannelGroup* GetMasterGroup() const { return masterGroup; }
	FMOD::ChannelGroup* GetBGMGroup() const { return bgmGroup; }
	FMOD::ChannelGroup* GetSFXGroup() const { return sfxGroup; }

	void SetMasterVolume(float volume);
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);
};

