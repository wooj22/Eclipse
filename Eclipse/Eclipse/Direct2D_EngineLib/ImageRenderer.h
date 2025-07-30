#pragma once
#include <wrl/client.h> 
#include <d2d1_1.h>
#include "IRenderer.h"
#include "RenderSystem.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "iostream"

/* [Image Renderer Conponent]
* <UI>의 이미지 한 장(sprite)의 render를 담당하는 component로
* 컴포넌트 생성시 RenderSystem에 등록되어 sprite를 게속 render한다.
* sprite가 있다면 sprite 출력, 없다면 white box 칠한다.
* render시 필요한 rectTransform은 이 컴포넌트가 등록된 오브젝트의 rectTransform을 포인터에 담아 활용한다.
*/

class RectTransform;
class ImageRenderer : public IRenderer
{
private:
	RectTransform* rectTransform;
	D2D1_RECT_F destRect;
public:
	shared_ptr<Sprite> sprite;	// 공유 자원  
private:
	// sprite가 없을 경우 box draw
	ComPtr<ID2D1SolidColorBrush> brush;
	D2D1_COLOR_F baseColor = D2D1::ColorF(D2D1::ColorF::White);

public:
	// component cycle
	ImageRenderer() { 
		rendertype = RenderType::UI;		// 렌더 타입 지정 (UI)	
	};
	~ImageRenderer() override = default;

	void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
	void Update() override final;
	void Render() override final;
	void OnDestroy_Inner() override final;

public:
	void SetBaseColor(const D2D1_COLOR_F& newColor);
};

