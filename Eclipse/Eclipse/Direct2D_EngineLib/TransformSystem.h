#pragma once
#include <vector>
#include "Singleton.h"
#include "ITransform.h"
using namespace std;

/* TransformSystem의 역할
* 1. tranform, rectTrasfrom이 자신의 matrix(local, world, screen)을 update하게 한다.
*/


class Transform;
class RectTransform;
class TransformSystem : public Singleton<TransformSystem>
{
private :
	vector<ITransform*> components;
	vector<ITransform*> pending_components;

public:
	// componenet
	void Regist(ITransform* component);
	void Unregist(ITransform* component);

	// component system
	void Update();
};

