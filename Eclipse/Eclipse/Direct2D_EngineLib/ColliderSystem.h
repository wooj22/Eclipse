#pragma once
#include <vector>
#include "Singleton.h"
#include "RaycastHit.h"
using namespace std;

/* ColliderSystem의 역할
* 1. Collider들의 bound update
* 2. collision cheak(sap) -> 이벤트 함수 호출 연결
* 3. RayCast 발생시 모든 콜라이더와의 hit 연산 결과 반환
*/

class ICollider;
class ColliderSystem : public Singleton<ColliderSystem>
{
private:
	vector<ICollider*> components;
	vector<ICollider*> pending_components;

public:
	// componenet
	void Regist(ICollider* component);
	void Unregist(ICollider* component);

	// component system
	void FixedUpdate();

public:
	// func
	// ray hit collision
	RaycastHit Raycast(const Ray& ray, float maxDistance);
};
