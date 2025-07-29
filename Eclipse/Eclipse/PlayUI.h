#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/MultiDelegate.h"

class PlayUI : public GameObject
{
public:
	UI_Text* timerText;			// 웨이브 타이머
	UI_Button* stopButton;		// 일시 정지 버튼
	UI_Image* skillTree;		// 스킬 트리 이미지 추후 기능에 따라 변경 해야함

	UI_Image* questImage;		// 퀘스트창 이미지
	UI_Text* questText;			// 퀘스트창 텍스트

	UI_Image* chatImage;		// 대화창 이미지
	UI_Text* chatText;			// 대화창 텍스트

public:
	// game object cycle
	PlayUI() {}
	~PlayUI() override {}

	// 오브젝트가 생성될 때
	void Awake() override;

	// Scene의 Start (씬의 모든 오브젝트가 생성되고 난 뒤)
	void SceneStart() override;

	// Scene의 Update
	void Update()  override;

	// Scene의 Exit, GameObject Delete
	void Destroyed() override;
};
