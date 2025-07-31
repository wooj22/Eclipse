#include "SpriteRenderer.h"
#include "Transform.h"
#include "GameObject.h"

#ifndef D2D1_COLOR_MATRIX_PROP_COLOR_MATRIX
#define D2D1_COLOR_MATRIX_PROP_COLOR_MATRIX 0
#endif

void SpriteRenderer::OnEnable_Inner()
{
	RenderSystem::Get().Regist(this);
	transform = this->gameObject->transform;

	// effect 생성
	HRESULT hr = RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);
	hr = RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1Crop, &cropEffect);
	SUCCEEDED(hr);
}

void SpriteRenderer::OnDisable_Inner()
{
	RenderSystem::Get().Unregist(this);
	transform = nullptr;
}

void SpriteRenderer::OnDestroy_Inner()
{
	RenderSystem::Get().Unregist(this);
	sprite = nullptr;
	colorMatrixEffect = nullptr;
	cropEffect = nullptr;
}

void SpriteRenderer::Update() 
{
	// bound udpate
	if (!transform || !sprite) return;

	Vector2 worldPos = transform->GetWorldPosition();
	Vector2 size = Vector2(sprite->size.width, sprite->size.height);

	// pivot 보정 (pivot: 0~1 기준)
	boundPos = worldPos - Vector2(size.x * sprite->pivot.x, size.y * sprite->pivot.y) + 
		Vector2(size.x * 0.5f, size.y * 0.5f);
	boundSize = size;
}

void SpriteRenderer::Render() 
{
	if (!transform || !sprite) return;

	// source rect, size
	D2D1_RECT_F srcRect = sprite->sourceRect;
	D2D1_SIZE_F spriteSize = sprite->size;

	// pivox
	float pivotX = sprite->pivot.x;
	float pivotY = 1.0f - sprite->pivot.y;

	// dest rect
	destRect = {
		-spriteSize.width * pivotX,
		-spriteSize.height * pivotY,
		spriteSize.width * (1.0f - pivotX),
		spriteSize.height * (1.0f - pivotY)
	};

	// filp
	float scaleX = flipX ? -1.0f : 1.0f;
	float scaleY = flipY ? -1.0f : 1.0f;
	D2D1::Matrix3x2F flipMat = D2D1::Matrix3x2F::Scale(scaleX, scaleY, { 0, 0 });
	auto finalMat = flipMat * transform->GetScreenMatrix();

	// transform
	RenderSystem::Get().renderTarget->SetTransform(finalMat);
	
	// crop
	cropEffect->SetInput(0, sprite->texture->texture2D.Get());
	cropEffect->SetValue(D2D1_CROP_PROP_RECT, srcRect);

	// render
	// 1. draw bitmap
	//RenderSystem::Get().renderTarget->DrawBitmap(
	//	sprite->texture->texture2D.Get(),
	//	destRect,           // 출력 위치 및 크기
	//	alpha,				// 불투명도
	//	D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
	//	srcRect             // source rect
	//);
	
	// 2. draw image
	ComPtr<ID2D1Image> cropImage;
	cropEffect.As(&cropImage);

	colorMatrixEffect->SetInput(0, cropImage.Get());
	colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
	RenderSystem::Get().renderTarget->DrawImage(colorMatrixEffect.Get());
}

// Set Color
void SpriteRenderer::SetColor(float r, float g, float b, float a)
{
	// set RGBA
	colorMultiplier.r = r;
	colorMultiplier.g = g;
	colorMultiplier.b = b;
	colorMultiplier.a = a;

	// color maritx
	colorMatrix = {
	colorMultiplier.r, 0.0f,           0.0f,           0.0f,
	0.0f,           colorMultiplier.g, 0.0f,           0.0f,
	0.0f,           0.0f,           colorMultiplier.b, 0.0f,
	0.0f,           0.0f,           0.0f,           colorMultiplier.a
	};

	// multiply
	colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
	colorMatrixEffect->SetInput(0, sprite->texture->texture2D.Get());
}