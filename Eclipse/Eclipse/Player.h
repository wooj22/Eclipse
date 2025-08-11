#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "AfterImage.h"
#include "Shadow.h"



class Player : public GameObject
{
	Ray ray;
	RaycastHit hit;

public:
	
	// [ components ]
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	BoxCollider* collider;
	Animator* animator;
	AudioSource* audioSource;

	// [ animation asset ]
	PlayerAnimatorController* playerAnimatorController;

	// [ script ]
	PlayerFSM* playerFSM;

	// [ player setting ]
	float playerGravityScale = 100; 

	// [ Shadow ]
	GameObject* player_Shadow = nullptr; // 그림자 오브젝트
	Transform* shadow_transform = nullptr;
	SpriteRenderer* shadow_spriteRenderer = nullptr;


public:
	float GetPlayerGravityScale() const { return playerGravityScale; }

public:
	Player() : GameObject("Player", "Player")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<BoxCollider>();
		animator = AddComponent<Animator>();
		audioSource = AddComponent<AudioSource>();

		spriteRenderer->layer = 4;
		rigidbody->collisionDetection = CollisionDetection::Continuous;

		playerFSM = AddComponent<PlayerFSM>();

		// 그림자 생성
		player_Shadow = SceneManager::Get().GetCurrentScene()->CreateObject<Shadow>();

		// [ Audio ]
		audioSource->SetChannelGroup(AudioSystem::Get().GetSFXGroup());

	}
	~Player() override
	{
		delete playerAnimatorController;
	};

	void Awake() override 
	{
		// [ 캐릭터 애니메이션 초기화 ]
		playerAnimatorController = new PlayerAnimatorController();
		animator->SetController(playerAnimatorController);

		transform->SetPosition(-300, -500);
		transform->SetScale(0.5, 0.5);

		collider->offset = { 10.0f, -55.0f };
		collider->size = { 140.0f, 350.0f };

		rigidbody->useGravity = true;
		rigidbody->gravityScale = playerFSM->defaultGravity;
		rigidbody->mass = 1.4f; // 3.0f; 


		shadow_transform = player_Shadow->GetComponent<Transform>();
		shadow_spriteRenderer = player_Shadow->GetComponent<SpriteRenderer>();
	}


	void Update() override
	{
		UpdateShadow();

		// AABB 영역 
		collider->DebugColliderDraw();
	}

	void UpdateShadow()
	{
		if (!shadow_transform || !shadow_spriteRenderer)
		{ OutputDebugStringA("[Player] player_Shadow의 Component가 없습니다. \n"); return; }

		D2D1_POINT_2F start = { 0, 0 };
		D2D1_POINT_2F end = { 0, -240 };
		RenderSystem::Get().DebugDrawLine(start, end, transform->GetScreenMatrix(), 2.0f);

		// ray 
		ray.direction = Vector2::down;
		ray.origin = transform->GetWorldPosition() - Vector2(0, 120);
		hit = ColliderSystem::Get().Raycast(ray, 5000);

		if (!hit.collider || 
			hit.collider->gameObject->name == "Player" || 
			hit.collider->gameObject->name == "Wall" ||
			hit.collider->gameObject->tag == "BoxObject")
		{
			player_Shadow->GetComponent<SpriteRenderer>()->SetEnabled(false);
			return;
		}

		//std::string debugStr = "[PlayerFSM] hit.collider = " + hit.collider->gameObject->name + " / hit의 ray 좌표 = " + std::to_string(hit.point.y) + "\n";
		//OutputDebugStringA(debugStr.c_str());

		float distance = transform->GetWorldPosition().y - hit.point.y;
		float maxDistance = 300.0f;

		if (distance > maxDistance)
		{
			player_Shadow->GetComponent<SpriteRenderer>()->SetEnabled(false);
			return;
		}

		if (!player_Shadow->GetComponent<SpriteRenderer>()->IsEnabled())
			player_Shadow->GetComponent<SpriteRenderer>()->SetEnabled(true);

		float alpha = 1.0f - (distance / maxDistance);
		alpha = clamp(alpha, 0.0f, 1.0f);

		if(spriteRenderer->flipX)  player_Shadow->GetComponent<Transform>()->SetPosition({ transform->GetWorldPosition().x - 15.0f , hit.point.y - 7.0f });
		else player_Shadow->GetComponent<Transform>()->SetPosition({ transform->GetWorldPosition().x + 15.0f , hit.point.y - 7.0f });

		player_Shadow->GetComponent<SpriteRenderer>()->SetAlpha(alpha);
	}
};