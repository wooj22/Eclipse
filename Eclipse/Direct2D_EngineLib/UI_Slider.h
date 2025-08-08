#pragma once
#include "GameObject.h"
#include "RectTransform.h"
#include "ImageRenderer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Slider.h"

/* [UI_Slider GameObject]
* Slider UI GameObject는 여러 계층구조를 필요로 하므로 미리 선언해둔다.
* 게임 콘텐츠에서는 이 게임오브젝트를 Create하면 바로 Slider를 사용하면 된다.
*/

class UI_Slider : public GameObject
{
public:
	// componet
	RectTransform* rectTransform;
	Slider* slider;

	// child gameobject
	GameObject* child_background = nullptr;		// recttransform, imagerenderer
	GameObject* child_guage = nullptr;			// recttransform, imagerenderer(fill)
	GameObject* child_handle = nullptr;			// recttransform, imagerenderer

private:
	// child set ref
	RectTransform* child_background_Rect = nullptr;
	RectTransform* child_guage_Rect = nullptr;
	RectTransform* child_handle_Rect = nullptr;

	ImageRenderer* child_background_Image = nullptr;
	ImageRenderer* child_guage_Image = nullptr;
	ImageRenderer* child_handle_Image = nullptr;

public:
	UI_Slider()
	{
		rectTransform = AddComponent<RectTransform>();
		slider = AddComponent <Slider>();
		rectTransform->SetSize(300, 30);

		// child_background
		child_background = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
		child_background_Rect = child_background->AddComponent<RectTransform>();
		child_background_Image = child_background->AddComponent<ImageRenderer>();
		child_background_Rect->SetParent(rectTransform);
		child_background_Rect->SetSize(rectTransform->GetSize().width, rectTransform->GetSize().height * 0.4);

		auto t1 = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Box.png");
		auto s1 = ResourceManager::Get().CreateSprite(t1, "s1");
		child_background_Image->sprite = s1;
		child_background_Image->renderMode = RenderMode::UnlitColorTint;
		child_background_Image->SetColor(0.5, 0.5, 0.5);

		// child_guage
		child_guage = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
		child_guage_Rect = child_guage->AddComponent<RectTransform>();
		child_guage_Image = child_guage->AddComponent<ImageRenderer>();
		child_guage_Rect->SetParent(rectTransform);
		child_guage_Rect->SetSize(rectTransform->GetSize().width, rectTransform->GetSize().height * 0.4);

		auto t2 = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Box.png");
		auto s2 = ResourceManager::Get().CreateSprite(t2, "s2");
		child_guage_Image->sprite = s2;
		child_guage_Image->fillType = FillType::Horizontal;
		child_guage_Image->fillAmount = 0.5;

		// child_handle
		child_handle = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
		child_handle_Rect = child_handle->AddComponent<RectTransform>();
		child_handle_Image = child_handle->AddComponent<ImageRenderer>();
		child_handle_Rect->SetParent(rectTransform);
		child_handle_Rect->SetSize(rectTransform->GetSize().height * 0.8, rectTransform->GetSize().height * 0.8);

		auto t3 = ResourceManager::Get().CreateTexture2D("..//Resource/Sample/Circle.png");
		auto s3 = ResourceManager::Get().CreateSprite(t3, "s3");
		child_handle_Image->sprite = s3;
		
		// slider compoent ref
		slider->slideArea = rectTransform;
		slider->gauge = child_guage_Image;
		slider->handle = child_handle_Rect;
	}
	~UI_Slider() override {}

public:
	// UI Size는 바로 rectTranform을 접근하면 안되고, 아래 함수를 통해 만들어야합니다.
	// Slider size에 따라 하위 오브젝트들 사이즈가 기본으로 setting
	void SetSliderSize(float w, float h)
	{
		rectTransform->SetSize(w, h);;
		child_background_Rect->SetSize(w, h * 0.4);
		child_guage_Rect->SetSize(w, h * 0.4);
		child_handle_Rect->SetSize(h * 0.8, h * 0.8);
	}
	
	// handle
	void SetHandleSize(float s)
	{
		child_handle_Rect->SetSize(s, s);
	}

	// UI Sprite 변경은 아래 함수를 통해서 합니다.
	// background sprite
	void SetBackgroundImage(shared_ptr<Sprite> new_sprite)
	{
		child_background_Image->sprite = new_sprite;
	}

	// gauge sprite
	void SetGuageImage(shared_ptr<Sprite> new_sprite)
	{
		child_guage_Image->sprite = new_sprite;
	}

	// handle sprite
	void SetHandleImage(shared_ptr<Sprite> new_sprite)
	{
		child_handle_Image->sprite = new_sprite;
	}
};