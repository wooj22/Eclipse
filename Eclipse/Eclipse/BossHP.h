#pragma once
#include"../Direct2D_EngineLib/SceneManager.h"
#include"../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "GameManager.h"

class BossHP : public GameObject
{

public:
	UI_Image* hpBarEmpty;
	UI_Image* hpBarFill;

public:
	BossHP() = default;
	~BossHP() = default;

	void Awake() override;           
	void SceneStart() override;
	void Update() override;         
	void Destroyed() override; 
	void RefreshBossHp(float hp);
};

