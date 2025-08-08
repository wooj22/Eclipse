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

	UI_Slider()
	{
		rectTransform = AddComponent<RectTransform>();
		slider = AddComponent <Slider>();
		rectTransform->SetSize(200, 30);

		// child_background
		child_background = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
		child_background->AddComponent<RectTransform>();
		child_background->AddComponent<ImageRenderer>();
		child_background->rectTransform->SetParent(rectTransform);

		auto t1 = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Box.png");
		auto s1 = ResourceManager::Get().CreateSprite(t1, "s1");
		child_background->GetComponent<ImageRenderer>()->sprite = s1;
		child_background->rectTransform->SetSize(rectTransform->GetSize().width, rectTransform->GetSize().height * 0.4);
		child_background->GetComponent<ImageRenderer>()->renderMode = RenderMode::UnlitColorTint;
		child_background->GetComponent<ImageRenderer>()->SetColor(0.5, 0.5, 0.5);

		// child_guage
		child_guage = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
		child_guage->AddComponent<RectTransform>();
		child_guage->AddComponent<ImageRenderer>();
		child_guage->rectTransform->SetParent(rectTransform);

		auto t2 = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Box.png");
		auto s2 = ResourceManager::Get().CreateSprite(t2, "s2");
		child_guage->GetComponent<ImageRenderer>()->sprite = s2;
		child_guage->rectTransform->SetSize(rectTransform->GetSize().width, rectTransform->GetSize().height * 0.4);
		child_guage->GetComponent<ImageRenderer>()->fillType = FillType::Horizontal;
		child_guage->GetComponent<ImageRenderer>()->fillAmount = 0.5;


		// child_handle
		child_handle = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
		child_handle->AddComponent<RectTransform>();
		child_handle->AddComponent<ImageRenderer>();
		child_handle->rectTransform->SetParent(rectTransform);

		auto t3 = ResourceManager::Get().CreateTexture2D("..//Resource/Sample/Circle.png");
		auto s3 = ResourceManager::Get().CreateSprite(t3, "s3");
		child_handle->GetComponent<ImageRenderer>()->sprite = s3;
		child_handle->rectTransform->SetSize(30,30);

		// slider compoent ref
		slider->gauge = child_guage->GetComponent<ImageRenderer>();
		slider->handle = child_handle->GetComponent<ImageRenderer>();
	}
	~UI_Slider() override {}
};