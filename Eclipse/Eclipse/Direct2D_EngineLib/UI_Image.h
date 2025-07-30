#pragma once
#include "GameObject.h"
#include "RectTransform.h"
#include "ImageRenderer.h"

/* [UI_Image GameObject]
* 단일 Image에 필요한 멤버를 추가해둔 기본 UI_Image GameObject
*/

class UI_Image : public GameObject
{
public:
	RectTransform* rectTransform;
	ImageRenderer* imageRenderer;

	UI_Image() 
	{ 
		rectTransform = AddComponent<RectTransform>();
		imageRenderer = AddComponent <ImageRenderer>(); 
	}
	~UI_Image() override {  }

	void Awake() override {}
	void SceneStart() override {}
	void Update() override {}
	void Destroyed() override {}
};

