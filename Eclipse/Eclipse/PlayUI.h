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

// 사운드
#include "../Direct2D_EngineLib/AudioSystem.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

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

	// audio
	shared_ptr<AudioClip> bgmClip_Main = nullptr;
	shared_ptr<AudioClip> bgmClip_Wave = nullptr;
	shared_ptr<AudioClip> bgmClip_Boss = nullptr;
	//shared_ptr<AudioClip> bgmClip_Clear = nullptr;
	shared_ptr<AudioClip> sfxClip_Button1 = nullptr;
	shared_ptr<AudioClip> sfxClip_Button2 = nullptr;
	shared_ptr<AudioClip> sfxClip_SkillUI = nullptr;
	shared_ptr<AudioClip> sfxClip_SkillActive = nullptr;
	//shared_ptr<AudioClip> sfxClip_GameOver = nullptr;
	shared_ptr<AudioClip> sfxClip_ChangeScene = nullptr;

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

	UI_Image* skill1_Image;					//스킬1 이미지
	UI_Image* skill1Icon_Image;				//스킬1 아이콘 이미지
	UI_Image* skill1Key_Image;				//스킬1 텍스트
	UI_Image* skill1CollTimeFilter_Image;	//스킬1 아이콘 이미지

	UI_Image* skill2_Image;			// 스킬2 이미지
	UI_Image* skill2Icon_Image;		// 스킬2 이미지
	UI_Image* skill2Key_Image;		// 스킬1 텍스트

	UI_Text* waveInfo_Text;			// 웨이브 정보 텍스트

	tooltip* tooltip1;
	tooltip* tooltip2;

	UI_Button* skillWindowClose_Button;			// 스킬 창 검은 화면 이미지
	UI_Image* skillWindowBackGround_Image;			// 스킬 창 검은 화면 이미지
	UI_Image* skillWindowBackGroundGradient_Image;	// 스킬 창 그라데이션 이미지
	UI_Image* skillWindow_Image;					// 스킬 창 이미지
	UI_Image* skillWindowSplitter_Image;			// 스킬 창 이미지
	UI_Image* skillWindowName_Image;				// 스킬창 이름 이미지
	std::vector<SkillWindowButton*> skillButtons;	// 스킬 버튼 모음
	UI_Image* skillHonBig_Image;					// 스킬창 혼 이미지
	UI_Image* skillHon_Image;						// 스킬창 혼 이미지
	UI_Text* skillHon_Text;							// 스킬창 혼 개수 텍스트

	BossHP* bossHP;
	NPC* npc;

	//Audio
	AudioSource* bgmSource = nullptr;
	AudioSource* sfxSource = nullptr;

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

	void SkillActiveSound();
	void ButtonEnterSound();
	void ButtonClickSound();
};
