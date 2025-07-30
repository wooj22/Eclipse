#pragma once
#include "Vector2.h"
class ICollider;

/* Ray : 광선 */
struct Ray
{
	Vector2 origin;				// ray 시작점
	Vector2 direction;			// ray 방향 벡터
};


/* RayCastHit : 광선과 콜라이더의 충돌 정보 */
struct RaycastHit
{
	Vector2 point;				// 충돌 지점
	float distance;			    // Ray 시작점부터 충돌 지점까지 거리
	ICollider* collider;		// 충돌한 콜라이더
};

