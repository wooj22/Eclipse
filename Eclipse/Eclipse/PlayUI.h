#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Singleton.h"

class PlayUI : public GameObject , public Singleton<PlayUI>
{
public:
	UI_Text* timer_Text;			// 웨이브 타이머

	UI_Button* stop_Button;			// 일시 정지 버튼

	UI_Image* skill_Tree;			// 스킬 트리 이미지 추후 기능에 따라 변경 해야함

	UI_Image* quest_Image;			// 퀘스트창 이미지
	UI_Text* quest_Text;			// 퀘스트창 텍스트

	UI_Image* chat_Image;			// 대화창 이미지
	UI_Text* chat_Text;				// 대화창 텍스트

	UI_Image* hon_Image;			// 혼 이미지
	UI_Text* hon_Text;				// 혼 개수 텍스트

	UI_Image* skill1_Image;			// 스킬1 이미지
	UI_Text* skill1_Text;			// 스킬1 텍스트

	UI_Image* skill2_Image;			// 스킬2 이미지
	UI_Text* skill2_Text;			// 스킬2 텍스트

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

	void OpenChat() { chat_Image->SetActive(true);}

	void CloseChat() { chat_Image->SetActive(false); }

	bool ChatActiveCheck() { return chat_Image->IsActive(); }
};
