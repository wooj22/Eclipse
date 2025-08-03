#pragma once
#include <fmod.hpp>
#include <vector>
#include "Singleton.h"
using namespace std;

/* AudioSystem의 역할
* 1. FMOD 시스템 초기화 및 업데이트
*/

class AudioSource;
class AudioSystem : public Singleton<AudioSystem>
{
private:
	FMOD::System* system = nullptr;

private:
	vector<AudioSource*> components;
	vector<AudioSource*> pending_components;

public:
	// componenet
	void Regist(AudioSource* component);
	void Unregist(AudioSource* component);

	// component system
	void Init();
	void Update();
	void UnInit();

public:
	FMOD::System* GetSystem() const { return system; }
};

