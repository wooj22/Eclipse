#include "HonController.h"

HonController::HonController()
{
	SFX_HonCollision = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Crash.wav");
	SFX_HonDestroy = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Destroy.wav");
	SFX_HonSplit = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Split.wav");
	SFX_HonMerge = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Merge.wav");
}

// 투명화 연출
void HonController::OpacityDirecting()
{
	float targetAlpha = alphaData[hp];
	float currentAlpha = sr->GetAlpha();
	float changeSpeed = 2;

	if (currentAlpha < targetAlpha)
		currentAlpha = min(currentAlpha + changeSpeed * Time::GetDeltaTime(), targetAlpha);
	else if (currentAlpha > targetAlpha)
		currentAlpha = max(currentAlpha - changeSpeed * Time::GetDeltaTime(), targetAlpha);

	sr->SetAlpha(currentAlpha);
}

// direction
void HonController::SetDirection(Vector2 dir)
{ 
	moveDirection = dir; 
}

Vector2 HonController::Getdirection() 
{ 
	return moveDirection; 
}

// size
void HonController::SetSize(float s)
{
	size = s;
	if (tr) tr->SetScale(size, size);
}

float HonController::GetSize() 
{ 
	return size; 
}

// collision move speed
void HonController::SetSpeedUpRate(float r) 
{ 
	collisionSpeed = collisionSpeedDefalut * r; 
}

// descent speed
void HonController::SetDescentSpeed(float s) 
{
	descentSpeed = s; 
}

float HonController::GetSDescentpeed()
{ 
	return descentSpeed;
}

// hp
void HonController::SetHp(int h) 
{
	hp = h; 
	//if(sr) sr->SetAlpha(alphaData[hp]);
}

int HonController::GetHp() 
{
	return hp; 
}

void HonController::TakeDamage(int damage)
{
	if (destroyPending) return;

	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		Die();
	}

	// effect
	//sr->SetAlpha(alphaData[hp]);
}

void HonController::Die()
{
	// wave1 quest
	GameManager::Get().ChangeQuestCount(1);

	// hon score
	if (honType == HonType::A)
	{
		if (isCollisionD) {}
		else if (is2A) GameManager::Get().ChangeHonCount(30);
		else GameManager::Get().ChangeHonCount(10);
	}
	else if (honType == HonType::B)
	{
		if (isCollisionD) {}
		else if(isb) GameManager::Get().ChangeHonCount(10);
		else GameManager::Get().ChangeHonCount(15);
	}
	else if (honType == HonType::C)
	{
		if (isCollisionD) {}
		else GameManager::Get().ChangeHonCount(5);
	}
	else if (honType == HonType::D)
	{
		GameManager::Get().ChangeHonCount(-10);
	}

	// component off
	//sr->SetEnabled(false);
	collider->SetEnabled(false);

	// sound
	audioSource->SetClip(SFX_HonDestroy);
	audioSource->PlayOneShot();

	// destroy
	destroyPending = true;
}

// collision moving flag
void HonController::CollisionEnd() 
{ 
	isCollisionMoving = false;
	collisionMovingDelta = 0; 
}

void HonController::CollisionStart()
{ 
	isCollisionMoving = true;
	collisionMovingDelta = 0; 
}

// HonC 끌어당기는 함수
void HonController::HonC_PullMe(Vector2 pos)
{
	pullDirection = (pos - tr->GetWorldPosition()).Normalized();
	isPullMoving = true;
}

// 플레이어 흡수
void HonController::Absorption()
{
	isAbsorption = true;
	if (collider) collider->SetEnabled(false);
}