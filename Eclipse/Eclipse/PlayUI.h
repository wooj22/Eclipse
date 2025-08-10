#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Singleton.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"
#include "SkillWindowButton.h"
#include "PauseWindow.h"
#include "BossHp.h"
#include "tooltip.h"
#include <vector>

class Chat;
class Quest;
class NPC;


class PlayUI : public GameObject
{
private:
	float waveInfoTimer = 0;
	float waveIntoTime = 5;

	float tooltipInfoTimer = 0;
	float tooltipInfoTime = 10;

	float fadeTime = 2.5f;

	std::vector<wstring> tooltipName; 

public:
	UI_Text* timer_Text;			// 웨이브 타이머

	UI_Button* stop_Button;			// 일시 정지 버튼
	PauseWindow* pauseWindow;
	std::vector<UI_Button*> pauseCheckButtos;

	UI_Image* skill_Tree;			// 스킬 트리 이미지 추후 기능에 따라 변경 해야함

	UI_Image* quest_Image;			// 퀘스트창 이미지
	UI_Text* questName_Text;		// 퀘스트창 제목
	UI_Text* quest_Text;			// 퀘스트창 텍스트
	UI_Text* questCount_Text;		// 퀘스트 미션 카운트 텍스트
	Quest* quest;					// 퀘스트 스크립트

	UI_Image* chat_Image;			// 대화창 이미지
	UI_Text* chat_Text;				// 대화창 텍스트
	UI_Button* chat_Button;			// 퀘스트 수락 버튼
	Chat* chat;						// 대화창 스크립트

	UI_Image* hon_Image;			// 혼 이미지
	UI_Text* hon_Text;				// 혼 개수 텍스트

	UI_Image* skill1_Image;			// 스킬1 이미지
	UI_Text* skill1_Text;			// 스킬1 텍스트

	UI_Image* skill2_Image;			// 스킬2 이미지

	UI_Text* waveInfo_Text;			// 웨이브 정보 텍스트

	tooltip* tooltip1;
	tooltip* tooltip2;

	UI_Image* skillWindow_Image;	// 스킬 창 이미지
	UI_Text* skillWindowName_Text;	// 스킬창 이름 텍스트
	std::vector<SkillWindowButton*> skillButtons;
	UI_Image* skillHon_Image;		// 스킬창 혼 이미지
	UI_Text* skillHon_Text;			// 스킬창 혼 개수 텍스트

	BossHP* bossHP;
	NPC* npc;

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


	void ChangeHonCountText();

	void ActivateAbsorb();
	void DeactivateAbsorb();

	void ChatSetActive(bool check) {
		chat_Image->SetActive(check);
		chat_Button->SetActive(false);
	}
	bool ChatActiveCheck() { return chat_Image->IsActive(); }
	void ClickChatButton();

	void AllSkillCheat();
	void ResetAllSkillCheat();
	void PlayerInteraction();

	void StartWaveInfo(int waveNumber);

	void CheckPauseUI();
};
