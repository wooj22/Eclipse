#pragma once
#include "GameObject.h"
#include "RectTransform.h"
#include "ScreenTextRenderer.h"

/* [UI_Text GameObject]
* 단일 ScreenText에 필요한 멤버를 추가해둔 기본 UI_Text GameObject
*/

class UI_Text : public GameObject
{
public:
	RectTransform* rectTransform;
	ScreenTextRenderer* screenTextRenderer;

	UI_Text() 
	{ 
		rectTransform = AddComponent<RectTransform>();
		screenTextRenderer = AddComponent <ScreenTextRenderer>();
	}
	~UI_Text() override { }

	void Awake() override {}
	void SceneStart() override {}
	void Update() override {}
	void Destroyed() override {}
};
