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
	UI_Text* timer_Text;			// ���̺� Ÿ�̸�

	UI_Button* stop_Button;			// �Ͻ� ���� ��ư
	PauseWindow* pauseWindow;
	std::vector<UI_Button*> pauseCheckButtos;

	UI_Image* skill_Tree;			// ��ų Ʈ�� �̹��� ���� ��ɿ� ���� ���� �ؾ���

	UI_Image* quest_Image;			// ����Ʈâ �̹���
	UI_Text* questName_Text;		// ����Ʈâ ����
	UI_Text* quest_Text;			// ����Ʈâ �ؽ�Ʈ
	UI_Text* questCount_Text;		// ����Ʈ �̼� ī��Ʈ �ؽ�Ʈ
	Quest* quest;					// ����Ʈ ��ũ��Ʈ

	UI_Image* chat_Image;			// ��ȭâ �̹���
	UI_Text* chat_Text;				// ��ȭâ �ؽ�Ʈ
	UI_Button* chat_Button;			// ����Ʈ ���� ��ư
	Chat* chat;						// ��ȭâ ��ũ��Ʈ

	UI_Image* hon_Image;			// ȥ �̹���
	UI_Text* hon_Text;				// ȥ ���� �ؽ�Ʈ

	UI_Image* skill1_Image;			// ��ų1 �̹���
	UI_Text* skill1_Text;			// ��ų1 �ؽ�Ʈ

	UI_Image* skill2_Image;			// ��ų2 �̹���

	UI_Text* waveInfo_Text;			// ���̺� ���� �ؽ�Ʈ

	tooltip* tooltip1;
	tooltip* tooltip2;

	UI_Image* skillWindow_Image;	// ��ų â �̹���
	UI_Text* skillWindowName_Text;	// ��ųâ �̸� �ؽ�Ʈ
	std::vector<SkillWindowButton*> skillButtons;
	UI_Image* skillHon_Image;		// ��ųâ ȥ �̹���
	UI_Text* skillHon_Text;			// ��ųâ ȥ ���� �ؽ�Ʈ

	BossHP* bossHP;
	NPC* npc;

public:
	// game object cycle
	PlayUI() : GameObject("PlayUI","PlayUI") {}
	~PlayUI() override {}

	// ������Ʈ�� ������ ��
	void Awake() override;
	// Scene�� Start (���� ��� ������Ʈ�� �����ǰ� �� ��)
	void SceneStart() override;
	// Scene�� Update
	void Update()  override;
	// Scene�� Exit, GameObject Delete
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
