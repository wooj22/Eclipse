#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/RectTransform.h"
#include "../Direct2D_EngineLib/ScreenTextRenderer.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include <vector>

class EndScene : public Scene
{
private:
	// 카메라
	GameObject* cam = nullptr;

	// 크레딧 텍스트들을 저장할 벡터
	std::vector<UI_Text*> credits;

	// 스크롤 관련 변수
	float scrollY = 0.0f;
	float scrollSpeed = 100.0f;  // 기본 스크롤 속도

public:
	// Scene 생명주기 함수들
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;
};