#pragma once
#include "../Direct2D_EngineLib/Script.h"

/*---------------------*/
/* Boss Bullet Controller.cs */
/*-------------------*/

class Transform;
class SpriteRenderer;
class Rigidbody;
class CircleCollider;
class Vector2;

class BulletController : public Script
{
private:
	// data
	float speed = 50;
	float palyer_deceleration = 0.5;

	// controll
	Vector2 direction;

	// ref component
	Transform* tr;

public:
	// script componet cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

public:
	// bullet functions
	void SetDirection(Vector2 dir) { direction = dir; }
	void SetSpeed(float s) { speed = s; }
};