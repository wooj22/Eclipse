#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/MultiDelegate.h"

class PlayUI : public GameObject
{
public:
	UI_Text* timerText;			// ���̺� Ÿ�̸�
	UI_Button* stopButton;		// �Ͻ� ���� ��ư
	UI_Image* skillTree;		// ��ų Ʈ�� �̹��� ���� ��ɿ� ���� ���� �ؾ���

	UI_Image* questImage;		// ����Ʈâ �̹���
	UI_Text* questText;			// ����Ʈâ �ؽ�Ʈ

	UI_Image* chatImage;		// ��ȭâ �̹���
	UI_Text* chatText;			// ��ȭâ �ؽ�Ʈ

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
};
