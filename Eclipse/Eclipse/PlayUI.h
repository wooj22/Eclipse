#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Singleton.h"

class PlayUI : public GameObject , public Singleton<PlayUI>
{
public:
	UI_Text* timer_Text;			// ���̺� Ÿ�̸�

	UI_Button* stop_Button;			// �Ͻ� ���� ��ư

	UI_Image* skill_Tree;			// ��ų Ʈ�� �̹��� ���� ��ɿ� ���� ���� �ؾ���

	UI_Image* quest_Image;			// ����Ʈâ �̹���
	UI_Text* quest_Text;			// ����Ʈâ �ؽ�Ʈ

	UI_Image* chat_Image;			// ��ȭâ �̹���
	UI_Text* chat_Text;				// ��ȭâ �ؽ�Ʈ

	UI_Image* hon_Image;			// ȥ �̹���
	UI_Text* hon_Text;				// ȥ ���� �ؽ�Ʈ

	UI_Image* skill1_Image;			// ��ų1 �̹���
	UI_Text* skill1_Text;			// ��ų1 �ؽ�Ʈ

	UI_Image* skill2_Image;			// ��ų2 �̹���
	UI_Text* skill2_Text;			// ��ų2 �ؽ�Ʈ

public:
	// game object cycle
	PlayUI() {}
	~PlayUI() override {}

	// ������Ʈ�� ������ ��
	void Awake() override;

	// Scene�� Start (���� ��� ������Ʈ�� �����ǰ� �� ��)
	void SceneStart() override;

	// Scene�� Update
	void Update()  override;

	// Scene�� Exit, GameObject Delete
	void Destroyed() override;

	void OpenChat() { chat_Image->SetActive(true);}

	void CloseChat() { chat_Image->SetActive(false); }

	bool ChatActiveCheck() { return chat_Image->IsActive(); }
};
