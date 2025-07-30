#pragma once
#include <vector>
#include "Singleton.h"
using namespace std;

/* AnimatorSystem의 역할
* 1. Animator 컴포넌트 udpate -> 각 애니메이션이 내부적으로 돌아감
*/

class Animator;
class AnimatorSystem : public Singleton<AnimatorSystem>
{
private:
	vector<Animator*> components;
	vector<Animator*> pending_components;

public:
	// componenet
	void Regist(Animator* component);
	void Unregist(Animator* component);

	// component system
	void Update();
};


