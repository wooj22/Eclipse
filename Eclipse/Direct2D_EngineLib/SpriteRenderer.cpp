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

	auto& srcRect = sprite->sourceRect;
	auto& spriteSize = sprite->size;
	float pivotX = sprite->pivot.x;
	float pivotY = sprite->pivot.y;

	// Crop 설정
	cropEffect->SetInput(0, sprite->texture->texture2D.Get());
	cropEffect->SetValue(D2D1_CROP_PROP_RECT, srcRect);

	ComPtr<ID2D1Image> croppedImage;
	cropEffect->GetOutput(&croppedImage);

	// ColorMatrix 설정
	colorMatrixEffect->SetInput(0, croppedImage.Get());
	colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);

	ComPtr<ID2D1Image> finalImage;
	colorMatrixEffect->GetOutput(&finalImage);

	// Flip 변환 행렬
	float scaleX = flipX ? -1.0f : 1.0f;
	float scaleY = flipY ? -1.0f : 1.0f;
	auto flipMat = D2D1::Matrix3x2F::Scale(D2D1::Size(scaleX, scaleY), { 0, 0 });

	// Pivot 변환 행렬
	D2D1_POINT_2F pivotOffset = { spriteSize.width * pivotX, spriteSize.height * pivotY };
	auto pivotMat = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);

	// 최종 변환 행렬 세팅
	auto finalMat = flipMat * pivotMat * transform->GetScreenMatrix();
	RenderSystem::Get().renderTarget->SetTransform(finalMat);

	// sourceRect 위치 보정
	D2D1_POINT_2F drawOffset = { 0, 0 };
	drawOffset.x = flipX ? (-srcRect.left - spriteSize.width) : -srcRect.left;
	drawOffset.y = flipY ? (-srcRect.top - spriteSize.height) : -srcRect.top;

	// 이미지 렌더링
	RenderSystem::Get().renderTarget->DrawImage(finalImage.Get(), drawOffset);
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