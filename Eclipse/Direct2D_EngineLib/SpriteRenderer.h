#pragma once
#include <wrl/client.h> 
#include <d2d1_1.h>
#include "IRenderer.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "iostream"
#include <d2d1effects_2.h>
#pragma comment(lib, "dxguid.lib")

/* [Sprite Renderer Conponent]
* <World>의 이미지 한 장(sprite)의 render를 담당하는 component로
* 컴포넌트 생성시 RenderSystem에 등록되어 sprite를 게속 render한다.
* render시 필요한 transform은 이 컴포넌트가 등록된 오브젝트의 transform을 포인터에 담아 활용한다
*/

class Transform;
class SpriteRenderer : public IRenderer
{
private:
	Transform* transform;
	D2D1_RECT_F destRect;

private:
	ComPtr<ID2D1Effect> colorMatrixEffect = nullptr;
	ColorRGBA colorMultiplier = { 1,1,1,1 };	// R, G, B, A
	D2D1::Matrix5x4F colorMatrix = 				// colorMatrix 곱셈 행렬
		{
			colorMultiplier.r, 0.0f,           0.0f,           0.0f,			0.0f,
			0.0f,           colorMultiplier.g, 0.0f,           0.0f,			0.0f,
			0.0f,           0.0f,           colorMultiplier.b, 0.0f,			0.0f,
			0.0f,           0.0f,           0.0f,           colorMultiplier.a,  0.0f
		};

public:
	shared_ptr<Sprite> sprite;		// 공유 자원	
	float alpha = 1.0f;				// 투명도
	bool flipX = false;				// x축 반전
	bool flipY = false;				// y축 반전

public:
	// component cycle
	SpriteRenderer() = default;
	~SpriteRenderer() override = default;

	void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
	void Update() override final;
	void Render() override final;
	void OnDestroy_Inner() override final;

public:
	// set
	void SetColor(float r, float g, float b, float a = 1.0f);
};

