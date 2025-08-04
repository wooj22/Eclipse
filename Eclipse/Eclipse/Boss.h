#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "BossController.h"

// audio test
#include "../Direct2D_EngineLib/AudioSystem.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/* ���� ���̺꿡 �����ϴ� Boss GameObject ���� Ŭ���� */

class Boss : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	BossController* bossController;

	// audio test
	AudioSource* bgmSource = nullptr;
	AudioSource* sfxSource = nullptr;
	shared_ptr<AudioClip> bgmClip = nullptr;
	shared_ptr<AudioClip> sfxClip = nullptr;

	Boss()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		bossController = AddComponent<BossController>();

		name = "Boss";
		tag = "Boss";

		transform->SetPosition(0, 500);
		transform->SetScale(1, 1);

		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Boss.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "Boss");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->layer = 10;

		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->radius = 180;
		collider->offset = { 0,-40 };

		// color matrix test
		spriteRenderer->renderMode = RenderMode::Lit_ColorTint;
		spriteRenderer->SetGlowAmmount(150);

		// audio test
		bgmSource = AddComponent<AudioSource>();
		sfxSource = AddComponent<AudioSource>();

		bgmClip = ResourceManager::Get().CreateAudioClip("../Resource/Audio/Sample/BGM_Stage1.wav");
		sfxClip = ResourceManager::Get().CreateAudioClip("../Resource/Audio/Sample/SFX_Score.wav");
	
		AudioSystem::Get().SetBGMVolume(0.5);

		bgmSource->SetChannelGroup(AudioSystem::Get().GetBGMGroup());
		bgmSource->SetVolume(1);
		bgmSource->SetClip(bgmClip);
		bgmSource->SetLoop(true);
		bgmSource->Play();

		sfxSource->SetClip(sfxClip);
		sfxSource->SetLoop(true);
		sfxSource->Play();
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

