#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "Options.h"

// 사운드
#include "../Direct2D_EngineLib/AudioSystem.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

class TitleUI : public GameObject
{
public:
	GameObject* backgroundImage;
	GameObject* titleLogo_Image;
	GameObject* titleSpell_Image;

	UI_Button* play_Button;
	UI_Button* options_Button;
	UI_Button* credit_Button;
	UI_Button* end_Button;
	UI_Image* underscore_Image;
	std::vector<UI_Button*> menuButtons;
	std::vector<UI_Button*> optionButtons;

	Options* optionUI;

	// hover event check
	UI_Button* prevHoverButton = nullptr;

	// audio
	AudioSource* bgmSource = nullptr;
	AudioSource* sfxSource = nullptr;
	shared_ptr<AudioClip> bgmClip = nullptr;
	shared_ptr<AudioClip> sfxClip_Button1 = nullptr;
	shared_ptr<AudioClip> sfxClip_Button2 = nullptr;
	shared_ptr<AudioClip> sfxClip_ChangeScene = nullptr;

public:
	// game object cycle
	TitleUI() {}
	~TitleUI() override {}

	// 오브젝트가 생성될 때
	void Awake() override;

	// Scene의 Start (씬의 모든 오브젝트가 생성되고 난 뒤)
	void SceneStart() override;

	// Scene의 Update
	void Update()  override;

	// Scene의 Exit, GameObject Delete
	void Destroyed() override;
	void OnPointEnterButton(UI_Button* parent);
	void OnPointExitButton(UI_Button* prevButton);
	void OnClickOptionUI(UI_Button* button);
	void OpenOptionUI();
	void ChangePlayScene();
};



