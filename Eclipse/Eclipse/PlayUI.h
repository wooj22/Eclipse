#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Singleton.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"

class Chat;
class Quest;


class PlayUI : public GameObject
{
private:
	float waveInfoTimer = 0;
	float waveIntoTime = 5;

	float tolltipInfoTimer = 0;
	float tolltipInfoTime = 10;

public:
	UI_Text* timer_Text;			// 웨이브 타이머

	UI_Button* stop_Button;			// 일시 정지 버튼

	UI_Image* skill_Tree;			// 스킬 트리 이미지 추후 기능에 따라 변경 해야함

	UI_Image* quest_Image;			// 퀘스트창 이미지
	UI_Text* quest_Text;			// 퀘스트창 텍스트
	//Quest* quest;

	UI_Image* chat_Image;			// 대화창 이미지
	UI_Text* chat_Text;				// 대화창 텍스트
	UI_Button* chat_Button;			// 퀘스트 수락 버튼
	Chat* chat;

	UI_Image* hon_Image;			// 혼 이미지
	UI_Text* hon_Text;				// 혼 개수 텍스트

	UI_Image* skill1_Image;			// 스킬1 이미지
	UI_Text* skill1_Text;			// 스킬1 텍스트

	UI_Image* skill2_Image;			// 스킬2 이미지
	UI_Text* skill2_Text;			// 스킬2 텍스트

	UI_Text* waveInfo_Text;			// 웨이브 정보 텍스트

	UI_Image* tooltip_Image;		// 툴팁 이미지 부모
	UI_Image* tooltipHon_Image;		// 툴팁 혼 이미지
	UI_Text* tooltipName_Text;		// 툴팁 이름 텍스트
	UI_Text* tooltipEx_Text;		// 툴팁 설명 텍스트

	UI_Image* tooltip2_Image;		// 툴팁 이미지
public:
	// game object cycle
	PlayUI() : GameObject("PlayUI","PlayUI") {}
	~PlayUI() override {}

	// 오브젝트가 생성될 때
	void Awake() override;

	// Scene의 Start (씬의 모든 오브젝트가 생성되고 난 뒤)
	void SceneStart() override;

	// Scene의 Update
	void Update()  override;

	// Scene의 Exit, GameObject Delete
	void Destroyed() override;


	void ChatSetActive(bool check) {
		chat_Image->SetActive(check);
		chat_Button->SetActive(false);
	}

	bool ChatActiveCheck() { return chat_Image->IsActive(); }

	void ClickChatButton() {
		GameManager::Get().isWave = true;
		GameManager::Get().waveCount++;
		chat_Button->SetActive(false);
		chat_Image->SetActive(false);
		waveInfo_Text->SetActive(true);

		//wave 별 이름 변화는 툴팁에서
		//tooltip_Image->SetActive(true);
	}
};
