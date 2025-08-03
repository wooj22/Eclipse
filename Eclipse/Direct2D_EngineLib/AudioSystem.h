#pragma once
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
	vector<AudioSource*> components;
	vector<AudioSource*> pending_components;

public:
	// componenet
	void Regist(AudioSource* component);
	void Unregist(AudioSource* component);

	// component system
	void Update();
};

