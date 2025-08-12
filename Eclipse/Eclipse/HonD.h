#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "HonDAnimatorController.h"
#include "HonDController.h"

/* Luna ´ÞºûÀÇ È¥ */

class HonD : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	Animator* animator;
	AudioSource* audioSource;
	HonDController* controller;

	// animator controller asset
	HonDAnimatorController* animatorController;

	HonD() : GameObject("HonD", "Hon")		// name, tag
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		animator = AddComponent<Animator>();
		audioSource = AddComponent<AudioSource>();
		controller = AddComponent<HonDController>();

		// animator
		animatorController = new HonDAnimatorController();
		animator->SetController(animatorController);

		// renderer set
		spriteRenderer->renderMode = RenderMode::Lit_Glow;
		spriteRenderer->layer = 11;

		// physics set
		rigidbody->useGravity = false;
		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->offset = { 0, -5 };
		collider->radius = 40;

		// audio chnnel set
		audioSource->SetChannelGroup(AudioSystem::Get().GetSFXGroup());
		audioSource->SetLoop(false);
	}

	~HonD()
	{
		delete animatorController;
	}
};

