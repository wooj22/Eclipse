#include "HonmunCollisionScript.h"
#include "Aron_Scene.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Time.h"
#include <random>
#include <algorithm>
#include <cmath>

void HonmunCollisionScript::Awake()
{
	// Honmun\uc774 GameObject\uc774\ubbc0\ub85c \uc548\uc804 \uce90\uc2a4\ud305
	honmun = dynamic_cast<Honmun*>(gameObject);

	// \ud544\uc694\ud55c \ucef4\ud3ec\ub10c\ud2b8\ub4e4 \ucc38\uc870 \uac00\uc838\uc624\uae30
	transform = gameObject->GetComponent<Transform>();
	rigidbody = gameObject->GetComponent<Rigidbody>();
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	
	// \uc18d\ub3c4 \ucd08\uae30\ud654
	currentVelocity.x = 0.0f;
	currentVelocity.y = 0.0f;
}

void HonmunCollisionScript::Start()
{
	// �ʱ� ����
	started = true;
	
	// 목표 크기가 설정되어 있으면 적용
	if (targetSize > 0.0f)
	{
		OutputDebugStringA("Applying target size in Start()\n");
		UpdateSize(targetSize);
		targetSize = -1.0f; // 적용 완료 표시
	}
}

void HonmunCollisionScript::Update()
{
	// \ucfe8\ub2e4\uc6b4 \ucc98\ub9ac
	if (reactionCooldown > 0)
	{
		reactionCooldown -= Time::GetDeltaTime();
	}

	// \ucda9\ub3cc \ud6c4 \uad00\uc131 \ud6a8\uacfc \uc801\uc6a9
	if (rigidbody && rigidbody->isKinematic && currentVelocity.Magnitude() > minVelocity)
	{
		// \ud604\uc7ac \uc704\uce58\uc5d0 \uc18d\ub3c4 \uc801\uc6a9
		Vector2 currentPos = transform->GetPosition();
		Vector2 newPos = currentPos + currentVelocity * Time::GetDeltaTime();
		transform->SetPosition(newPos.x, newPos.y);
		
		// \ub9c8\ucc30\ub825\uc73c\ub85c \uc18d\ub3c4 \uac10\uc18c
		currentVelocity = currentVelocity * friction;
	}
	else if (rigidbody && rigidbody->isKinematic)
	{
		// \ucd5c\uc18c \uc18d\ub3c4 \uc774\ud558\uc5d0\uc11c \uc815\uc9c0
		currentVelocity.x = 0.0f;
		currentVelocity.y = 0.0f;
	}

	// \uae30\uc874 \ub099\ud558 \uc18d\ub3c4 \ucc98\ub9ac (\ube44\ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc\uc5d0\uc11c\ub9cc)
	if (rigidbody && rigidbody->isKinematic)
	{
		// \ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc\uc5d0\uc11c\ub9cc \uc218\ub3d9 \ub099\ud558 \uc18d\ub3c4 \uc801\uc6a9
		Vector2 currentPos = transform->GetPosition();
		transform->SetPosition(currentPos.x, currentPos.y - fallingSpeed * Time::GetDeltaTime());
	}
}

void HonmunCollisionScript::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	if (other->gameObject->name == "Player")
	{
		Vector2 dir = other->gameObject->transform->GetWorldPosition() - this->gameObject->transform->GetWorldPosition();
		dir = dir.Normalized();

		// 혼문 체력 1 감소
		health--;
		
		// 디버그 출력
		char debugMsg[100];
		sprintf_s(debugMsg, "Player hit Honmun! Honmun health: %d -> %d\n", health + 1, health);
		OutputDebugStringA(debugMsg);
		
		// 체력이 0이 되면 파괴
		if (health <= 0)
		{
			OutputDebugStringA("Honmun destroyed by player!\n");
			
			// 점수 시스템을 위한 현재 씬 가져오기
			auto* currentScene = SceneManager::Get().GetCurrentScene();
			auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
			
			// 혼문 타입별 점수 부여
			if (aronScene)
			{
				int points = 0;
				switch (honmunType)
				{
				case HonmunType::A:
					// 2A인지 확인
					points = (currentSize > 15.0f) ? 3 : 1;
					break;
				case HonmunType::B:
					points = 1;
					break;
				case HonmunType::C:
					points = 0; // C는 플레이어가 파괴해도 점수 없음
					break;
				case HonmunType::D:
					points = -1; // D는 플레이어가 파괴하면 -1점
					break;
				}
				
				if (points != 0)
				{
					aronScene->AddScore(points);
					sprintf_s(debugMsg, "Player destroyed Honmun type %d, points: %d\n", (int)honmunType, points);
					OutputDebugStringA(debugMsg);
				}
			}
			
			DestroyThis();
			return;
		}
		
		// 플레이어를 밀어내는 효과 (선택사항)
		// 혼문이 플레이어 반대 방향으로 밀려남
		if (rigidbody && rigidbody->isKinematic)
		{
			float pushSpeed = 300.0f;
			currentVelocity = -dir * pushSpeed;
			reactionCooldown = 0.5f; // 짧은 쿨다운
		}
	}

	if (other->gameObject->name == "Honmun")
	{
		OutputDebugStringA("OnTriggerEnter called!\n");

		// \ucfe8\ub2e4\uc6b4 \uc911\uc774\uac70\ub098 \uc774\ubbf8 \ube44\ud65c\uc131\ud654\ub41c \uac1d\uccb4\ub294 \ucc98\ub9ac \uc548\ud568
		if (reactionCooldown > 0 || isProcessingReaction || !gameObject->IsActive())
		{
			OutputDebugStringA("Collision blocked by cooldown, processing, or inactive object!\n");
			return;
		}

		HonmunCollisionScript* otherScript = GetHonmunScript(other);
		if (!otherScript)
		{
			OutputDebugStringA("No HonmunCollisionScript found on other object!\n");
			return;
		}

		// \ucda9\ub3cc \uac1d\uccb4 \ud0c0\uc785 \ucd9c\ub825
		char debugMsg[100];
		sprintf_s(debugMsg, "Trigger: My type = %d, Other type = %d (Speeds: %.2f vs %.2f)\n",
			(int)honmunType, (int)otherScript->honmunType, currentVelocity.Magnitude(), otherScript->currentVelocity.Magnitude());
		OutputDebugStringA(debugMsg);

		// \ubc18\uc751 \ucc98\ub9ac \ud50c\ub798\uadf8 \uc124\uc815
		isProcessingReaction = true;
		otherScript->isProcessingReaction = true;

		// 점수 시스템을 위한 현재 씬 가져오기 (체력 체크 전에 미리 준비)
		auto* currentScene = SceneManager::Get().GetCurrentScene();
		auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);

		// 체력 1인 객체의 Mixed reaction 점수를 먼저 계산
		bool shouldCalculateMixedScore = (health == 1 || otherScript->health == 1) &&
			(honmunType != otherScript->honmunType);

		if (shouldCalculateMixedScore && aronScene)
		{
			// A<->B, A<->C, B<->C 충돌 시 +2점 (체력 1 객체 포함 시)
			if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::B) ||
				(honmunType == HonmunType::B && otherScript->honmunType == HonmunType::A) ||
				(honmunType == HonmunType::A && otherScript->honmunType == HonmunType::C) ||
				(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::A) ||
				(honmunType == HonmunType::B && otherScript->honmunType == HonmunType::C) ||
				(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::B))
			{
				aronScene->AddScore(2);
				OutputDebugStringA("Mixed collision with HP=1 object: Score +2 added early!\n");
			}
			// D와의 충돌은 -2점 (체력 1 객체 포함 시)
			else if (honmunType == HonmunType::D || otherScript->honmunType == HonmunType::D)
			{
				aronScene->AddScore(-2);
				OutputDebugStringA("D collision with HP=1 object: Score -2 added early!\n");
			}
		}

		// 체력 0인 객체는 어떤 충돌이든 즉시 파괴
		if (health <= 0 || otherScript->health <= 0)
		{
			OutputDebugStringA("Health <= 0 object detected, destroying!\n");
			if (health <= 0) DestroyThis();
			if (otherScript->health <= 0) otherScript->DestroyThis();
			return;
		}

		// \ud0c0\uc785\ubcc4 \ucda9\ub3bc \ucc98\ub9ac
		if (honmunType == otherScript->honmunType)
		{
			// \uac19\uc740 \ud0c0\uc785\ub07c\ub9ac \ucda9\ub3cc
			OutputDebugStringA("Same type collision detected!\n");
			switch (honmunType)
			{
			case HonmunType::A: // Ignis - \ud569\uccb4
				OutputDebugStringA("A + A collision!\n");
				HandleIgnisReaction(otherScript);
				break;
			case HonmunType::B: // Umbra - \uccb4\ub825 \uac10\uc18c
				OutputDebugStringA("B + B collision calling HandleUmbraReaction!\n");
				HandleUmbraReaction(otherScript);
				break;
			case HonmunType::C: // Darkness - \uccb4\ub825 \uac10\uc18c
				OutputDebugStringA("C + C collision!\n");
				HandleDarknessReaction(otherScript);
				break;
			case HonmunType::D: // Luna - \uccb4\ub825 \uac10\uc18c
				OutputDebugStringA("D + D collision!\n");
				HandleLunaReaction(otherScript);
				break;
			}
		}
		else
		{
			// �ٸ� Ÿ�Գ��� �浹
			HandleMixedReaction(otherScript);
		}

		// \ucfe8\ub2e4\uc6b4 \uc124\uc815 - B \ud0c0\uc785\uc740 \ub354 \uc9e7\uc740 \ucfe8\ub2e4\uc6b4
		float cooldownTime = (honmunType == HonmunType::B) ? 0.3f : 1.0f;
		reactionCooldown = cooldownTime;
		isProcessingReaction = false;
		if (otherScript && otherScript->gameObject && otherScript->gameObject->IsActive())
		{
			float otherCooldown = (otherScript->honmunType == HonmunType::B) ? 0.3f : 1.0f;
			otherScript->reactionCooldown = otherCooldown;
			otherScript->isProcessingReaction = false;
		}
	}
}

void HonmunCollisionScript::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// \ud0c0\uc785\ubcc4 \uae30\ubcf8 \uc18d\uc131 \uc124\uc815 - \ubaa8\ub4e0 \ud0c0\uc785 \uccb4\ub825 3\uc73c\ub85c \ud1b5\uc77c
	switch (type)
	{
	case HonmunType::A: // Ignis
		health = 3;
		currentSize = 10.0f;
		fallingSpeed = 1.0f;
		break;
	case HonmunType::B: // Umbra
		health = 3;
		currentSize = 10.0f;
		fallingSpeed = 1.0f;
		break;
	case HonmunType::C: // Darkness
		health = 3;
		currentSize = 10.0f;
		fallingSpeed = 1.0f;
		break;
	case HonmunType::D: // Luna
		health = 3;
		currentSize = 10.0f;
		fallingSpeed = 1.0f;
		break;
	}
}

void HonmunCollisionScript::HandleIgnisReaction(HonmunCollisionScript* otherScript)
{
	// A + A = \uccb4\ub825 \uac10\uc18c + \ud569\uccb4 \uc2dc\uc2a4\ud15c (체력 2 이상 남으면 합체)
	
	// 점수 시스템을 위한 현재 씬 가져오기
	auto* currentScene = SceneManager::Get().GetCurrentScene();
	auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
	
	// \uccb4\ub825 \uac10\uc18c
	health--;
	otherScript->health--;
	
	// \uccb4\ub825\uc774 0\uc774 \ub418\uba74 \uc644\uc804 \ud30c\uad34 + 점수 부여
	if (health <= 0 || otherScript->health <= 0)
	{
		OutputDebugStringA("A type health reached 0, destroying objects!\n");
		
		if (health <= 0) 
		{
			// 파괴되는 A가 2A인지 확인 (HP=1이고 크기가 큰 경우)
			if (aronScene)
			{
				bool is2A = (currentSize > 15.0f && health + 1 == 1); // 체력감소 전이 1이었다면 2A
				int points = is2A ? 3 : 1;
				aronScene->AddScore(points);
				
				char debugMsg[100];
				sprintf_s(debugMsg, "A destroyed! Was 2A: %s, Points: %d\n", is2A ? "Yes" : "No", points);
				OutputDebugStringA(debugMsg);
			}
			DestroyThis();
		}
		
		if (otherScript->health <= 0) 
		{
			// 파괴되는 A가 2A인지 확인
			if (aronScene)
			{
				bool is2A = (otherScript->currentSize > 15.0f && otherScript->health + 1 == 1);
				int points = is2A ? 3 : 1;
				aronScene->AddScore(points);
				
				char debugMsg[100];
				sprintf_s(debugMsg, "A destroyed! Was 2A: %s, Points: %d\n", is2A ? "Yes" : "No", points);
				OutputDebugStringA(debugMsg);
			}
			otherScript->DestroyThis();
		}
	}
	else
	{
		// 체력 감소 후 둘 다 1 이상 남으면 합체 가능
		if (health >= 1 && otherScript->health >= 1)
		{
			OutputDebugStringA("Both A have health >= 2, merging into 2A!\n");
			
			// 합체: 둘 다 사라지고 새로운 2A(HP=1) 생성
			Vector2 midPosition = (transform->GetPosition() + otherScript->transform->GetPosition()) * 0.5f;
			
			// 새로운 2A 생성
			auto* scene = SceneManager::Get().GetCurrentScene();
			if (scene)
			{
				auto* new2A = scene->CreateObject<Honmun>();
				new2A->SetHonmunType(HonmunType::A);
				new2A->SetPosition(midPosition.x, midPosition.y);
				new2A->SetSize(1.5f); // 2A 크기로 설정
				
				auto* newScript = new2A->GetComponent<HonmunCollisionScript>();
				if (newScript)
				{
					newScript->SetHealth(1); // 2A는 HP=1
					newScript->currentSize = 15.0f; // 2A 크기
					
					OutputDebugStringA("New 2A created with HP=1!\n");
				}
			}
			
			// 기존 A들 파괴
			otherScript->DestroyThis();
			DestroyThis();
		}
		else
		{
			// 체력이 부족하면 합체 안하고 튕김만
			OutputDebugStringA("Not enough health for merge, bounce only!\n");
			BounceAwayKinematic(otherScript);
		}
	}
}

void HonmunCollisionScript::HandleUmbraReaction(HonmunCollisionScript* otherScript)
{
	// B + B = \uccb4\ub825 \uac10\uc18c + \ubd84\ud574 \uc2dc\uc2a4\ud15c (3 → 2 → 1 → 0 \uc644\uc804 \ud30c\uad34)
	
	// \ub514\ubc84\uadf8 \ucd9c\ub825
	OutputDebugStringA("B + B collision detected!\n");
	
	// \ubd84\ud574\ub41c \uc870\uac01\ub4e4 \ucc98\ub9ac: \ucda9\ub3bc \uc2dc \uc989\uc2dc \ud30c\uad34 + 점수 부여
	if (isSplitFragment || otherScript->isSplitFragment)
	{
		OutputDebugStringA("Split fragment collision, destroying immediately!\n");
		
		// 점수 시스템을 위한 현재 씬 가져오기
		auto* currentScene = SceneManager::Get().GetCurrentScene();
		auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
		
		if (isSplitFragment) 
		{
			// b 조각 파괴 시 +1점
			if (aronScene)
			{
				aronScene->AddScore(1);
				OutputDebugStringA("b fragment destroyed! Points: +1\n");
			}
			DestroyThis();
		}
		
		if (otherScript->isSplitFragment) 
		{
			// b 조각 파괴 시 +1점
			if (aronScene)
			{
				aronScene->AddScore(1);
				OutputDebugStringA("b fragment destroyed! Points: +1\n");
			}
			otherScript->DestroyThis();
		}
		return;
	}
	
	// \uc6d0\ub798 B \uac1d\uccb4\ub4e4\ub9cc \ubd84\ud574 \uc2dc\uc2a4\ud15c \uc218\ud589
	// \uccb4\ub825 \uac10\uc18c
	health--;
	otherScript->health--;
	
	// \uccb4\ub825\uc774 0\uc774 \ub418\uba74 \uc644\uc804 \ud30c\uad34 + 점수 부여
	if (health <= 0 || otherScript->health <= 0)
	{
		OutputDebugStringA("B type health reached 0, destroying objects!\n");
		
		// 점수 시스템을 위한 현재 씬 가져오기
		auto* currentScene = SceneManager::Get().GetCurrentScene();
		auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
		
		if (health <= 0) 
		{
			// B 파괴 시 +1점
			if (aronScene)
			{
				aronScene->AddScore(1);
				OutputDebugStringA("B destroyed! Points: +1\n");
			}
			DestroyThis();
		}
		
		if (otherScript->health <= 0) 
		{
			// B 파괴 시 +1점
			if (aronScene)
			{
				aronScene->AddScore(1);
				OutputDebugStringA("B destroyed! Points: +1\n");
			}
			otherScript->DestroyThis();
		}
	}
	else
	{
		// \uccb4\ub825\uc774 \ub0a8\uc544\uc788\uc73c\uba74 \ubd84\ud574 \uc2dc\uc2a4\ud15c \uc791\ub3d9
		OutputDebugStringA("Both original B objects still have health, splitting with bounce!\n");
		
		// \ucda9\ub3bc \ud6c4 \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc (\uc54c\uae4c\uae30 \ucef4\uc149)
		// BB 충돌 보정 제거로 충돌 감지 개선
		// BounceAwayKinematic(otherScript);
		
		// \ubd84\ud574: \ucd1d 4\uac1c \uc0dd\uc131 (\uae30\uc874 2\uac1c\ub294 \ud30c\uad34\ub418\uace0 \uc0c8\ub85c 4\uac1c \uc0dd\uc131)
		CreateSplitObjectsWithCollision(4, otherScript);
		
		otherScript->DestroyThis();
		DestroyThis();
	}
}

void HonmunCollisionScript::HandleDarknessReaction(HonmunCollisionScript* otherScript)
{
	// C + C = \uccb4\ub825 \uac10\uc18c \uc2dc\uc2a4\ud15c (3 → 2 → 1 → 0 \uc644\uc804 \ud30c\uad34) + 점수 +3점
	
	// \ub514\ubc84\uadf8 \ucd9c\ub825
	OutputDebugStringA("C + C collision detected!\n");
	
	// 점수 시스템을 위한 현재 씬 가져오기
	auto* currentScene = SceneManager::Get().GetCurrentScene();
	auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
	
	// C + C 충돌 시 항상 +3점 부여
	if (aronScene)
	{
		aronScene->AddScore(3);
		OutputDebugStringA("C + C collision: +3 points!\n");
	}
	
	// \uccb4\ub825 \uac10\uc18c
	health--;
	otherScript->health--;
	
	// \uccb4\ub825\uc774 0\uc774 \ub418\uba74 \uc644\uc804 \ud30c\uad34
	if (health <= 0 || otherScript->health <= 0)
	{
		OutputDebugStringA("C type health reached 0, destroying objects!\n");
		if (health <= 0) DestroyThis();
		if (otherScript->health <= 0) otherScript->DestroyThis();
	}
	else
	{
		// \uccb4\ub825\uc774 \ub0a8\uc544\uc788\uc73c\uba74 \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc (\uc54c\uae4c\uae30 \ucef4\uc149)
		OutputDebugStringA("Both C objects still have health, strong bounce!\n");
		
		// \ucda9\ub3bc \ud6c4 \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc
		BounceAwayKinematic(otherScript);
	}
}

void HonmunCollisionScript::HandleLunaReaction(HonmunCollisionScript* otherScript)
{
	// D + D = \uccb4\ub825 \uac10\uc18c \uc2dc\uc2a4\ud15c (3 → 2 → 1 → 0 \uc644\uc804 \ud30c\uad34)
	
	// \ub514\ubc84\uadf8 \ucd9c\ub825
	OutputDebugStringA("D + D collision detected!\n");
	
	// \uccb4\ub825 \uac10\uc18c
	health--;
	otherScript->health--;
	
	// \uccb4\ub825\uc774 0\uc774 \ub418\uba74 \uc644\uc804 \ud30c\uad34
	if (health <= 0 || otherScript->health <= 0)
	{
		OutputDebugStringA("D type health reached 0, destroying objects!\n");
		if (health <= 0) DestroyThis();
		if (otherScript->health <= 0) otherScript->DestroyThis();
	}
	else
	{
		// \uccb4\ub825\uc774 \ub0a8\uc544\uc788\uc73c\uba74 \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc (\uc54c\uae4c\uae30 \ucef4\uc149)
		OutputDebugStringA("Both D objects still have health, strong bounce!\n");
		
		// \ucda9\ub3bc \ud6c4 \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc
		BounceAwayKinematic(otherScript);
	}
}

void HonmunCollisionScript::HandleMixedReaction(HonmunCollisionScript* otherScript)
{
	// 점수 시스템을 위한 현재 씬 가져오기
	auto* currentScene = SceneManager::Get().GetCurrentScene();
	auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
	
	// 체력 1인 객체가 있었는지 확인 (이미 점수가 추가되었는지 체크)
	bool wasHP1Collision = (health == 1 || otherScript->health == 1);
	
	// \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8: A\uc640 B\ub9cc \uc0ac\uc6a9
	// A&B \ub610\ub294 B&A - \ud295\uae40 (\ud0a4\ub124\ub9c8\ud2f1\uc73c\ub85c \ubcc0\uacbd) + A 체력 감소 + 점수 +1
	if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::B) ||
		(honmunType == HonmunType::B && otherScript->honmunType == HonmunType::A))
	{
		// 점수 +1 추가 (체력 1 객체가 아닌 경우에만 - 이미 추가되지 않았을 때만)
		if (aronScene && !wasHP1Collision)
		{
			aronScene->AddScore(1);
			OutputDebugStringA("A<->B collision: Score +1 added!\n");
		}
		
		// A와 B 모두 체력 감소 + 파괴 시 추가 점수
		if (honmunType == HonmunType::A) 
		{
			// 2A인지 확인 (파괴 전에)
			bool is2A = (currentSize > 15.0f && health == 1);
			
			health--;
			if (health <= 0) 
			{
				// 2A가 파괴되면 추가 +3점
				if (is2A && aronScene)
				{
					aronScene->AddScore(3);
					OutputDebugStringA("2A destroyed in A<->B collision: +3 bonus points!\n");
				}
				DestroyThis();
				return;
			}
		}
		else if (honmunType == HonmunType::B)
		{
			// B 타입도 체력 감소
			health--;
			if (health <= 0)
			{
				OutputDebugStringA("B destroyed in A<->B collision!\n");
				DestroyThis();
				return;
			}
		}
		
		if (otherScript->honmunType == HonmunType::A) 
		{
			// 2A인지 확인 (파괴 전에)
			bool is2A = (otherScript->currentSize > 15.0f && otherScript->health == 1);
			
			otherScript->health--;
			if (otherScript->health <= 0) 
			{
				// 2A가 파괴되면 추가 +3점
				if (is2A && aronScene)
				{
					aronScene->AddScore(3);
					OutputDebugStringA("2A destroyed in A<->B collision: +3 bonus points!\n");
				}
				otherScript->DestroyThis();
				return;
			}
		}
		else if (otherScript->honmunType == HonmunType::B)
		{
			// B 타입도 체력 감소
			otherScript->health--;
			if (otherScript->health <= 0)
			{
				OutputDebugStringA("B destroyed in A<->B collision!\n");
				otherScript->DestroyThis();
				return;
			}
		}
		
		BounceAwayKinematic(otherScript);
	}
	// A&C 또는 C&A - 점수 +1
	else if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::C) ||
		(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::A))
	{
		if (aronScene && !wasHP1Collision)
		{
			aronScene->AddScore(1);
			OutputDebugStringA("A<->C collision: Score +1 added!\n");
		}
		
		// A와 C 모두 체력 감소
		if (honmunType == HonmunType::A) 
		{
			bool is2A = (currentSize > 15.0f && health == 1);
			health--;
			if (health <= 0) 
			{
				if (is2A && aronScene)
				{
					aronScene->AddScore(3);
					OutputDebugStringA("2A destroyed in A<->C collision: +3 bonus points!\n");
				}
				DestroyThis();
				return;
			}
		}
		else if (honmunType == HonmunType::C)
		{
			health--;
			if (health <= 0)
			{
				DestroyThis();
				return;
			}
		}
		
		if (otherScript->honmunType == HonmunType::A) 
		{
			bool is2A = (otherScript->currentSize > 15.0f && otherScript->health == 1);
			otherScript->health--;
			if (otherScript->health <= 0) 
			{
				if (is2A && aronScene)
				{
					aronScene->AddScore(3);
					OutputDebugStringA("2A destroyed in A<->C collision: +3 bonus points!\n");
				}
				otherScript->DestroyThis();
				return;
			}
		}
		else if (otherScript->honmunType == HonmunType::C)
		{
			otherScript->health--;
			if (otherScript->health <= 0)
			{
				otherScript->DestroyThis();
				return;
			}
		}
		
		BounceAwayKinematic(otherScript);
	}
	// B&C 또는 C&B - 점수 +1
	else if ((honmunType == HonmunType::B && otherScript->honmunType == HonmunType::C) ||
		(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::B))
	{
		if (aronScene && !wasHP1Collision)
		{
			aronScene->AddScore(1);
			OutputDebugStringA("B<->C collision: Score +1 added!\n");
		}
		
		// B와 C 모두 체력 감소
		if (honmunType == HonmunType::B) 
		{
			health--;
			if (health <= 0) 
			{
				DestroyThis();
				return;
			}
		}
		else if (honmunType == HonmunType::C)
		{
			health--;
			if (health <= 0)
			{
				DestroyThis();
				return;
			}
		}
		
		if (otherScript->honmunType == HonmunType::B) 
		{
			otherScript->health--;
			if (otherScript->health <= 0) 
			{
				otherScript->DestroyThis();
				return;
			}
		}
		else if (otherScript->honmunType == HonmunType::C)
		{
			otherScript->health--;
			if (otherScript->health <= 0)
			{
				otherScript->DestroyThis();
				return;
			}
		}
		
		BounceAwayKinematic(otherScript);
	}
	// D와의 충돌 - 항상 -1점 (충돌하는 객체 파괴)
	else if (honmunType == HonmunType::D || otherScript->honmunType == HonmunType::D)
	{
		if (aronScene && !wasHP1Collision)
		{
			aronScene->AddScore(-1);
			OutputDebugStringA("D collision: Score -1!\n");
		}
		
		// D가 아닌 객체 파괴
		if (honmunType != HonmunType::D)
		{
			DestroyThis();
		}
		if (otherScript->honmunType != HonmunType::D)
		{
			otherScript->DestroyThis();
		}
	}
}

// MergeWithOther 함수는 더 이상 사용하지 않음 - 새로운 2A 생성 방식으로 변경

void HonmunCollisionScript::SplitIntoTwo()
{
	// ũ�� 30% ����
	UpdateSize(currentSize * 0.7f);

	// ���ϼӵ� 20% ����
	UpdateFallingSpeed(1.2f);

	// ���ο� ȥ�� ����
	Vector2 currentPos = transform->GetPosition();
	Vector2 offset = GetRandomDirection() * 2.0f;

	auto* newHonmun = Instantiate<Honmun>(currentPos + offset);
	auto* newScript = newHonmun->AddComponent<HonmunCollisionScript>();
	newScript->SetHonmunType(honmunType);
	newScript->UpdateSize(currentSize);
	newScript->UpdateFallingSpeed(fallingSpeed);
	newScript->SetHealth(1); // 에너지 1로 설정
}

void HonmunCollisionScript::CreateSplitObjectsWithCollision(int count, HonmunCollisionScript* otherScript)
{
	Vector2 myPos = transform->GetPosition();
	Vector2 otherPos = otherScript->transform->GetPosition();
	
	// 충돌 방향 계산
	Vector2 collisionDirection = (myPos - otherPos).Normalized();
	
	OutputDebugStringA("CreateSplitObjectsWithCollision started!\n");

	// 각각 2개씩 쪼개지는 효과
	for (int i = 0; i < count; i++)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<float> distanceDis(200.0f, 300.0f); // \uac70\ub9ac \ub300\ud3ed \uc99d\uac00
		static std::uniform_real_distribution<float> spreadDis(60.0f, 120.0f); // \ud37c\uc9d0 \uac01\ub3c4 \ub300\ud3ed \uc99d\uac00
		
		Vector2 baseDirection;
		float spreadAngle = spreadDis(gen) * (3.14159f / 180.0f); // 30-60도 랜덤
		
		if (i < 2) 
		{
			// 첫 번째 객체의 조각들 - 내 방향으로
			float side = (i == 0) ? 1.0f : -1.0f; // 좌우로 분산
			baseDirection.x = collisionDirection.x * cos(spreadAngle * side) - collisionDirection.y * sin(spreadAngle * side);
			baseDirection.y = collisionDirection.x * sin(spreadAngle * side) + collisionDirection.y * cos(spreadAngle * side);
		}
		else 
		{
			// 두 번째 객체의 조각들 - 상대방 방향으로
			Vector2 oppositeDirection = -collisionDirection;
			float side = (i == 2) ? 1.0f : -1.0f; // 좌우로 분산
			baseDirection.x = oppositeDirection.x * cos(spreadAngle * side) - oppositeDirection.y * sin(spreadAngle * side);
			baseDirection.y = oppositeDirection.x * sin(spreadAngle * side) + oppositeDirection.y * cos(spreadAngle * side);
		}
		
		float distance = distanceDis(gen);
		Vector2 direction = baseDirection.Normalized();
		
		// 방향벡터만 계산 (거리는 속도로 사용)
		auto* scene = SceneManager::Get().GetCurrentScene();
		if (!scene) 
		{
			OutputDebugStringA("Scene is null!\n");
			continue;
		}

		OutputDebugStringA("Creating new Honmun object...\n");
		auto* newHonmun = scene->CreateObject<Honmun>();
		if (!newHonmun)
		{
			OutputDebugStringA("Failed to create Honmun!\n");
			continue;
		}
		
		newHonmun->SetHonmunType(honmunType);
		// \uc801\uc808\ud55c \uc704\uce58\uc5d0\uc11c \uc2dc\uc791 - \ub354 \uba40\ub9ac \ub5a8\uc5b4\uc9c4 \uc704\uce58
		Vector2 startPos = (i < 2) ? myPos : otherPos;
		float largeOffset = 80.0f;  // \uc624\ud504\uc14b \ub300\ud3ed \uc99d\uac00
		newHonmun->SetPosition(startPos.x + direction.x * largeOffset, startPos.y + direction.y * largeOffset);
		
		// \uc0c8\ub85c\uc6b4 \uac1d\uccb4\uc5d0 \uc18d\ub3c0 \ubd80\uc5ec \ubc0f \uc124\uc815
		auto* newScript = newHonmun->GetComponent<HonmunCollisionScript>();
		auto* newRigidbody = newHonmun->GetComponent<Rigidbody>();
		if (newScript && newRigidbody)
		{
			// SetHonmunType 이후에 크기를 다시 설정해야 함
			float smallSize = currentSize * 0.7f; // 원래 크기의 70%
			char debugMsg[200];
			sprintf_s(debugMsg, "Creating split object %d: originalSize=%.2f, smallSize=%.2f\n", i, currentSize, smallSize);
			OutputDebugStringA(debugMsg);
			
			// 크기를 저장하고 나중에 적용하도록 설정
			newScript->currentSize = smallSize;
			newScript->targetSize = smallSize; // 목표 크기 설정
			newScript->UpdateFallingSpeed(fallingSpeed * 1.2f);
			// \ubd84\ud574\ub41c \uc870\uac01\ub4e4\uc740 \uccb4\ub825 1\ub85c \uc124\uc815 (\ucda9\ub3cc \uc2dc \uc989\uc2dc \ud30c\uad34)
			newScript->SetHealth(1);
			newScript->isSplitFragment = true;  // \ubd84\ud574\ub41c \uc870\uac01\uc784\uc744 \ud45c\uc2dc
			
			// 분열 조각은 물리 모드로 설정 (중력으로 떨어지게)
			newRigidbody->isKinematic = false;
			newRigidbody->useGravity = true;
			
			// \ubd84\ud574 \uc2dc \ub9e4\uc6b0 \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc (\uc54c\uae4c\uae30 \ucef4\uc149)
			Vector2 splitVelocity;
			splitVelocity.x = direction.x * distance * 3.0f;  // \ubb3c\ub9ac \uc2dc\uc2a4\ud15c\uc5d0 \ub9de\uac8c \uac15\ub3c4 \uc870\uc808
			splitVelocity.y = direction.y * distance * 3.0f;
			// \ubb3c\ub9ac \ubaa8\ub4dc\uc5d0\uc11c\ub294 currentVelocity \ub300\uc2e0 rigidbody\uc5d0 \uc9c1\uc811 \uc18d\ub3c4 \uc124\uc815
			newRigidbody->velocity = splitVelocity;
			
			// \ub354 \uba00\ub9ac \ub098\uac00\ub3c4\ub85d \ub9c8\ucc30\ub825 \ub300\ud3ed \uac10\uc18c
			newScript->friction = 0.97f;  // \ub9c8\ucc30\ub825 \ub354 \uac10\uc18c
			
			sprintf_s(debugMsg, "Applied kinematic velocity %d: x=%.2f, y=%.2f\n", i, splitVelocity.x, splitVelocity.y);
			OutputDebugStringA(debugMsg);
			
			// \ubd84\ud574 \uc9c1\ud6c4 \ucda9\ub3cc \ubc29\uc9c0\ub97c \uc704\ud55c \uc9e7\uc740 \ucfe8\ub2e4\uc6b4 (\ubd84\ud574\ub41c \uc870\uac01\ub4e4\ub9cc)
			newScript->reactionCooldown = 0.5f;  // \ubd84\ud574\ub41c \uc870\uac01\ub4e4\uc740 0.5\ucd08 \uc9e7\uc740 \uba74\uc5ed
			
			OutputDebugStringA("Split object created successfully!\n");
		}
		else
		{
			OutputDebugStringA("Failed to get HonmunCollisionScript from new object!\n");
		}
	}
	
	OutputDebugStringA("CreateSplitObjectsWithCollision completed!\n");
}

void HonmunCollisionScript::CreateSplitObjects(int count)
{
	Vector2 currentPos = transform->GetPosition();
	OutputDebugStringA("CreateSplitObjects started!\n");

	for (int i = 0; i < count; i++)
	{
		// 4방향으로 고정 분산 (상하좌우 + 대각선)
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<float> distanceDis(200.0f, 300.0f); // \uac70\ub9ac \ub300\ud3ed \uc99d\uac00
		
		// 4\uac1c \uc870\uac01\uc744 4\ubc29\ud5a5\uc73c\ub85c \ud655\uc2e4\ud788 \ubd84\ub9ac - \ub354 \ub113\uac8c \ud37c\uc9d0
		float baseAngles[4] = {0.0f, 90.0f, 180.0f, 270.0f}; // 동, 북, 서, 남
		float angle = (baseAngles[i] + (gen() % 90 - 45)) * (3.14159f / 180.0f); // ±45\ub3c4 \ub7a8\ub364 (\ub354 \ub113\uac8c)
		float distance = distanceDis(gen);  // 100-180 픽셀 랜덤 거리
		
		// 방향벡터만 계산 (거리는 속도로 사용)
		Vector2 direction;
		direction.x = cos(angle);
		direction.y = sin(angle);
		
		auto* scene = SceneManager::Get().GetCurrentScene();
		if (!scene) 
		{
			OutputDebugStringA("Scene is null!\n");
			continue;
		}

		OutputDebugStringA("Creating new Honmun object...\n");
		auto* newHonmun = scene->CreateObject<Honmun>();
		if (!newHonmun)
		{
			OutputDebugStringA("Failed to create Honmun!\n");
			continue;
		}
		
		newHonmun->SetHonmunType(honmunType);
		// \uc6d0\ub798 \uc704\uce58\uc5d0\uc11c \uc2dc\uc791 - \ub354 \uba40\ub9ac \ub5a8\uc5b4\uc9c4 \ucd08\uae30 \uc624\ud504\uc14b
		float largeOffset = 80.0f;  // \uc624\ud504\uc14b \ub300\ud3ed \uc99d\uac00
		newHonmun->SetPosition(currentPos.x + direction.x * largeOffset, currentPos.y + direction.y * largeOffset);
		
		// \uc0c8\ub85c\uc6b4 \uac1d\uccb4\uc5d0 \uc18d\ub3c0 \ubd80\uc5ec \ubc0f \uc124\uc815
		auto* newScript = newHonmun->GetComponent<HonmunCollisionScript>();
		auto* newRigidbody = newHonmun->GetComponent<Rigidbody>();
		if (newScript && newRigidbody)
		{
			// SetHonmunType 이후에 크기를 다시 설정해야 함
			float smallSize = currentSize * 0.7f; // 원래 크기의 70%
			char debugMsg[200];
			sprintf_s(debugMsg, "Creating split object: originalSize=%.2f, smallSize=%.2f\n", currentSize, smallSize);
			OutputDebugStringA(debugMsg);
			
			// 크기를 저장하고 나중에 적용하도록 설정
			newScript->currentSize = smallSize;
			newScript->targetSize = smallSize; // 목표 크기 설정
			newScript->UpdateFallingSpeed(fallingSpeed * 1.2f); // \ub099\ud558\uc18d\ub3c4 20% \uc99d\uac00
			newScript->SetHealth(1); // \uc5d0\ub108\uc9c0 1\ub85c \uc124\uc815
			
			// 분열 조각은 물리 모드로 설정 (중력으로 떨어지게)
			newRigidbody->isKinematic = false;   // 물리 모드
			newRigidbody->useGravity = true;     // 중력 활성화
			
			// \ubd84\ud574 \uc2dc \uac15\ud55c \ubc00\ub824\ub0a8 \ud6a8\uacfc - currentVelocity\ub85c \ud0a4\ub124\ub9c8\ud2f1 \uc6c0\uc9c1\uc784 (\uc54c\uae4c\uae30 \ucef4\uc149)
			Vector2 splitVelocity;
			splitVelocity.x = direction.x * distance * 3.0f;  // \ubb3c\ub9ac \uc2dc\uc2a4\ud15c\uc5d0 \ub9de\uac8c \uac15\ub3c4 \uc870\uc808
			splitVelocity.y = direction.y * distance * 3.0f;
			// \ubb3c\ub9ac \ubaa8\ub4dc\uc5d0\uc11c\ub294 currentVelocity \ub300\uc2e0 rigidbody\uc5d0 \uc9c1\uc811 \uc18d\ub3c4 \uc124\uc815
			newRigidbody->velocity = splitVelocity;
			
			// \ub354 \uba00\ub9ac \ub098\uac00\ub3c4\ub85d \ub9c8\ucc30\ub825 \ub300\ud3ed \uac10\uc18c
			newScript->friction = 0.97f;  // \ub9c8\ucc30\ub825 \ub354 \uac10\uc18c  // 적당한 마찰력으로 자연스러운 멈춤
			
			sprintf_s(debugMsg, "Applied kinematic velocity: x=%.2f, y=%.2f\n", splitVelocity.x, splitVelocity.y);
			OutputDebugStringA(debugMsg);
			
			// \ubd84\ud574 \uc9c1\ud6c4 \ucda9\ub3cc \ubc29\uc9c0\ub97c \uc704\ud55c \uae34 \ucfe8\ub2e4\uc6b4
			newScript->reactionCooldown = 2.0f;  // 2\ucd08 \ub3d9\uc548 \ucda9\ub3cc \ube44\ud65c\uc131\ud654
			
			OutputDebugStringA("Split object created successfully!\n");
		}
		else
		{
			OutputDebugStringA("Failed to get HonmunCollisionScript from new object!\n");
		}
	}
	
	OutputDebugStringA("CreateSplitObjects completed!\n");
}

void HonmunCollisionScript::AbsorbNearbyEnemies(const Vector2& collisionPoint)
{
	// 플레이어 점프 거리 정도의 범위 설정 (약 300~400 픽셀)
	float attractionRadius = 350.0f;
	
	OutputDebugStringA("C+C Absorption effect activated!\n");
	
	// 현재 씬 가져오기
	auto* currentScene = SceneManager::Get().GetCurrentScene();
	auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
	if (!aronScene) return;
	
	// 모든 활성 혼문들을 검사
	auto& honmunManager = aronScene->honmunManager;
	std::vector<Honmun*> honmunsToDestroy;
	
	for (auto* honmun : honmunManager.activeHonmuns)
	{
		if (!honmun || !honmun->IsActive()) continue;
		
		auto* honmunScript = honmun->GetComponent<HonmunCollisionScript>();
		if (!honmunScript || honmunScript == this || honmunScript == otherScript) continue;
		
		// 거리 계산
		Vector2 honmunPos = honmun->transform->GetPosition();
		float distance = (collisionPoint - honmunPos).Magnitude();
		
		// 범위 내에 있는 혼문들 처리
		if (distance <= attractionRadius)
		{
			// 디버그 출력
			char debugMsg[200];
			sprintf_s(debugMsg, "Honmun in range! Type: %d, Distance: %.2f\n", 
				(int)honmunScript->honmunType, distance);
			OutputDebugStringA(debugMsg);
			
			// 충돌 지점으로 끌어당기기
			Vector2 direction = (collisionPoint - honmunPos).Normalized();
			float pullSpeed = 800.0f; // 강한 끌어당김
			
			// 키네마틱 모드로 이동
			if (honmunScript->rigidbody)
			{
				honmunScript->rigidbody->isKinematic = true;
				honmunScript->currentVelocity = direction * pullSpeed;
			}
			
			// 파괴 예약 (1초 후)
			honmunScript->reactionCooldown = 1.0f;
			honmunsToDestroy.push_back(honmun);
		}
	}
	
	// 끌어당긴 혼문들 파괴 처리
	for (auto* honmun : honmunsToDestroy)
	{
		auto* script = honmun->GetComponent<HonmunCollisionScript>();
		if (script)
		{
			// 점수 부여 (타입별로)
			if (aronScene)
			{
				int points = 0;
				switch (script->honmunType)
				{
				case HonmunType::A:
					points = (script->currentSize > 15.0f) ? 3 : 1;
					break;
				case HonmunType::B:
					points = 1;
					break;
				case HonmunType::C:
					points = 0; // C끼리는 이미 점수를 받았으므로
					break;
				case HonmunType::D:
					points = -1;
					break;
				}
				
				if (points != 0)
				{
					aronScene->AddScore(points);
					char debugMsg[100];
					sprintf_s(debugMsg, "Absorbed Honmun type %d destroyed, points: %d\n", 
						(int)script->honmunType, points);
					OutputDebugStringA(debugMsg);
				}
			}
			
			// 즉시 파괴
			script->health = 0;
			script->DestroyThis();
		}
	}
	
	char debugMsg[100];
	sprintf_s(debugMsg, "Total Honmuns absorbed: %d\n", (int)honmunsToDestroy.size());
	OutputDebugStringA(debugMsg);
}

void HonmunCollisionScript::DestroyThis()
{
	if (gameObject)
	{
		// \ucda9\ub3cc\uc790 \ube44\ud65c\uc131\ud654\ub85c \ucda9\ub3cc \ubc29\uc9c0
		auto* collider = gameObject->GetComponent<ICollider>();
		if (collider)
		{
			collider->isTrigger = true; // \ucda9\ub3cc \ubb34\uc2dc\ub418\ub3c4\ub85d \uc124\uc815
		}
		
		// \uc2a4\ud504\ub77c\uc774\ud2b8 \ube44\ud65c\uc131\ud654\ub85c \uc2dc\uac01\uc801 \uc228\uae40
		if (spriteRenderer)
		{
			spriteRenderer->alpha = 0.0f;
		}
		
		// \uac1d\uccb4 \ube44\ud65c\uc131\ud654
		gameObject->SetActive(false);
		
		OutputDebugStringA("Object completely destroyed!\n");
	}
}

void HonmunCollisionScript::BounceAway(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// �������� �ݴ�� ƨ�ܳ�
	Vector2 bounceDirection = -contact.normal;
	Vector2 bounceForce = bounceDirection * pushDistance;

	rigidbody->AddImpulse(bounceForce);
	otherScript->rigidbody->AddImpulse(-bounceForce);
}

void HonmunCollisionScript::BounceAwayKinematic(HonmunCollisionScript* otherScript)
{
	OutputDebugStringA("A & B kinematic bounce collision!\n");
	
	// \ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc\ub85c \ubcc0\uacbd
	if (rigidbody) rigidbody->isKinematic = true;
	if (otherScript->rigidbody) otherScript->rigidbody->isKinematic = true;

	// \ucda9\ub3cc \ubc29\ud5a5 \uacc4\uc0b0
	Vector2 myPos = transform->GetPosition();
	Vector2 otherPos = otherScript->transform->GetPosition();
	Vector2 collisionDirection = (myPos - otherPos).Normalized();
	
	// \ubc18\ub300 \ubc29\ud5a5\uc73c\ub85c \uc18d\ub3c4 \uc124\uc815 - \uc54c\uae4c\uae30 \ucef4\uc149\uc5d0 \ub9de\uac8c \uac15\ud654
	float bounceSpeed = 800.0f;  // \ud295\uae40 \uc18d\ub3c4 \ub300\ud3ed \uc99d\uac00 (\uc54c\uae4c\uae30 \ucef4\uc149)
	currentVelocity = collisionDirection * bounceSpeed;
	otherScript->currentVelocity = -collisionDirection * bounceSpeed;
	
	OutputDebugStringA("Objects bounced with velocity-based motion!\n");
}

void HonmunCollisionScript::PushSideways(HonmunCollisionScript* otherScript)
{
	// �¿�� �о
	Vector2 leftDirection(-1.0f, 0.0f);
	Vector2 rightDirection(1.0f, 0.0f);

	rigidbody->AddImpulse(leftDirection * pushDistance);
	otherScript->rigidbody->AddImpulse(rightDirection * pushDistance);
}

void HonmunCollisionScript::PassThrough(HonmunCollisionScript* otherScript)
{
	// ���� - ������ �浹�� ���������� ��ġ���� �ʰ� ��
	// �Ͻ������� �ݶ��̴��� Ʈ���ŷ� �����ϰų� ��ġ�� �ణ ����
	Vector2 separationDirection = (transform->GetPosition() - otherScript->transform->GetPosition()).Normalized();
	Vector2 separationForce = separationDirection * 1.0f;

	rigidbody->AddImpulse(separationForce);
	otherScript->rigidbody->AddImpulse(-separationForce);
}

std::vector<HonmunCollisionScript*> HonmunCollisionScript::GetNearbyHonmuns(float radius)
{
	std::vector<HonmunCollisionScript*> nearbyHonmuns;

	// 현재 씬의 모든 게임오브젝트에서 HonmunCollisionScript를 가진 것들을 찾기
	auto currentScene = SceneManager::Get().GetCurrentScene();
	if (!currentScene) return nearbyHonmuns;

	Vector2 myPosition = transform->GetPosition();
	
	// Scene의 모든 오브젝트를 순회하며 HonmunCollisionScript를 찾기 (임시 구현)
	// 실제로는 Scene의 GetAllObjects() 같은 함수가 필요하지만 없으므로 간단히 구현
	
	return nearbyHonmuns;
}

Vector2 HonmunCollisionScript::GetRandomDirection()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	return Vector2(dis(gen), dis(gen)).Normalized();
}

void HonmunCollisionScript::UpdateSize(float newSize)
{
	char debugMsg[200];
	sprintf_s(debugMsg, "UpdateSize called: currentSize=%.2f, newSize=%.2f\n", currentSize, newSize);
	OutputDebugStringA(debugMsg);
	
	currentSize = newSize;
	if (transform)
	{
		float scaleFactor = newSize / 10.0f; // �⺻ ũ�⸦ 10���� ����
		transform->SetScale(scaleFactor, scaleFactor);
		
		sprintf_s(debugMsg, "Scale applied: scaleFactor=%.2f\n", scaleFactor);
		OutputDebugStringA(debugMsg);
	}
	else
	{
		OutputDebugStringA("Transform is null in UpdateSize!\n");
	}
}

void HonmunCollisionScript::UpdateFallingSpeed(float speedMultiplier)
{
	fallingSpeed *= speedMultiplier;
}

bool HonmunCollisionScript::IsInCameraView()
{
	// ī�޶� ���� ���� �ִ��� Ȯ���ϴ� ����
	// ���� ������ ī�޶� �ý��ۿ� ���� �޶��� �� �ֽ��ϴ�
	return true; // �ӽ�
}

HonmunCollisionScript* HonmunCollisionScript::GetHonmunScript(ICollider* collider)
{
	if (!collider || !collider->gameObject) return nullptr;

	return collider->gameObject->GetComponent<HonmunCollisionScript>();
}