#pragma once
#include <vector>
#include "Singleton.h"
using namespace std;

/* PhysicsSystem의 역할
* 1. Rigidbody 컴포넌트의 FixedUpdate() -> 물리 연산 진행 -> transform udpate
*/

class Rigidbody;
class PhysicsSystem : public Singleton<PhysicsSystem>
{
private:
	vector<Rigidbody*> components;
	vector<Rigidbody*> pending_components;

public:
	// componenet
	void Regist(Rigidbody* component);
	void Unregist(Rigidbody* component);

	// component system
	void FixedUpdate();
};

