#pragma once
#include <fmod.hpp>
#include <vector>
#include "Singleton.h"
using namespace std;

/* AudioSystem의 역할
* 1. FMOD 시스템 초기화 및 업데이트
* 2. masterGroup, bgmGroup, sfxGroup 기본 채널 그룹 제공
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
	FMOD::ChannelGroup* ambGroup = nullptr;

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
	FMOD::ChannelGroup* GetAMBGroup() const { return ambGroup; }

	void SetMasterVolume(float volume);
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);
	void SetAMBVolume(float volume);
};

