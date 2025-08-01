#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include <vector>

// Forward declaration for Honmun class
class Honmun;

class Aron_Scene : public Scene
{
public:
	// Scene��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	// camera
	GameObject* cam = nullptr;

	// UI
	UI_Text* title_text = nullptr;
	UI_Text* score_text = nullptr;
	UI_Text* debug_text = nullptr;

	// \ud63c\ubb38 enemies (Honmun \ud074\ub798\uc2a4 \uc0ac\uc6a9) - \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8: A, B\ub9cc \uc0ac\uc6a9
	Honmun* honmun_a = nullptr;
	Honmun* honmun_b = nullptr;
	// Honmun* honmun_c = nullptr;  // \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc5d0\uc11c \uc8fc\uc11d\ucc98\ub9ac
	// Honmun* honmun_d = nullptr;  // \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc5d0\uc11c \uc8fc\uc11d\ucc98\ub9ac
	
	// 오브젝트 선택 시스템
	std::vector<Honmun*> allHonmuns;
	int selectedHonmunIndex = 0;

	// ground for enemies to stand on
	GameObject* ground = nullptr;
	BoxCollider* ground_col = nullptr;

	// 점수 시스템
	int currentScore = 0;
	
	// collision detection function
	void CheckCollisionAndChangeColor();
	void HandleHonmunMovement();
	
public:
	// 점수 관련 함수들 (다른 클래스에서 접근 가능)
	void AddScore(int points);
	int GetScore() const { return currentScore; }
	void UpdateScoreUI();
};