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
	RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);
	RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1Crop, &cropEffect);
	RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect);
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
	blurEffect = nullptr;
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

	// sprite
	D2D1_RECT_F srcRect = sprite->sourceRect;
	D2D1_SIZE_F spriteSize = sprite->size;

	if (renderMode == RenderMode::Unlit)
	{
		// pivox
		float pivotX = sprite->pivot.x;
		float pivotY = 1.0f - sprite->pivot.y;

		// dest rect
		D2D1_RECT_F destRect = {
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

		// render
		RenderSystem::Get().renderTarget->SetTransform(finalMat);
		RenderSystem::Get().renderTarget->DrawBitmap(
			sprite->texture->texture2D.Get(),
			destRect,           // 출력 위치 및 크기
			alpha,				// 불투명도
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			srcRect             // source rect
		);
	}
	else if (renderMode == RenderMode::UnlitColorTint)
	{
		float pivotX = sprite->pivot.x;
		float pivotY = sprite->pivot.y;

		// === Crop ===
		cropEffect->SetInput(0, sprite->texture->texture2D.Get());
		cropEffect->SetValue(D2D1_CROP_PROP_RECT, srcRect);
		ComPtr<ID2D1Image> croppedImage;
		cropEffect->GetOutput(&croppedImage);

		// === Color Matrix ===
		colorMatrixEffect->SetInput(0, croppedImage.Get());
		colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
		ComPtr<ID2D1Image> finalImage;
		colorMatrixEffect->GetOutput(&finalImage);

		// === Flip Matrix ===
		float scaleX = flipX ? -1.0f : 1.0f;
		float scaleY = flipY ? -1.0f : 1.0f;
		auto flipMat = D2D1::Matrix3x2F::Scale(D2D1::Size(scaleX, scaleY), { 0, 0 });

		// === Pivot Matrix ===
		D2D1_POINT_2F pivotOffset = { spriteSize.width * pivotX, spriteSize.height * pivotY };
		auto pivotMat = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);

		// === Fianl Matrix ===
		auto finalMat = flipMat * pivotMat * transform->GetScreenMatrix();
		RenderSystem::Get().renderTarget->SetTransform(finalMat);

		// === sourceRect 위치 보정 ===
		D2D1_POINT_2F drawOffset = { 0, 0 };
		drawOffset.x = flipX ? (-srcRect.left - spriteSize.width) : -srcRect.left;
		drawOffset.y = flipY ? (-srcRect.top - spriteSize.height) : -srcRect.top;

		// render
		RenderSystem::Get().renderTarget->DrawImage(finalImage.Get(), drawOffset);
	}
	else if (renderMode == RenderMode::Lit_Glow)
	{
		float pivotX = sprite->pivot.x;
		float pivotY = sprite->pivot.y;

		// === Crop ===
		cropEffect->SetInput(0, sprite->texture->texture2D.Get());
		cropEffect->SetValue(D2D1_CROP_PROP_RECT, srcRect);
		ComPtr<ID2D1Image> croppedImage;
		cropEffect->GetOutput(&croppedImage);

		// === Blur Source ===
		blurEffect->SetInput(0, croppedImage.Get());
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, blurAmmount);
		ComPtr<ID2D1Image> blurredImage;
		blurEffect->GetOutput(&blurredImage);

		// === Flip & Transform ===
		float scaleX = flipX ? -1.0f : 1.0f;
		float scaleY = flipY ? -1.0f : 1.0f;
		auto flipMat = D2D1::Matrix3x2F::Scale(D2D1::Size(scaleX, scaleY), { 0, 0 });

		D2D1_POINT_2F pivotOffset = { spriteSize.width * sprite->pivot.x, spriteSize.height * sprite->pivot.y };
		auto pivotMat = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
		auto finalMat = flipMat * pivotMat * transform->GetScreenMatrix();
		RenderSystem::Get().renderTarget->SetTransform(finalMat);

		// drawOffset 계산
		D2D1_POINT_2F drawOffset = { 0, 0 };
		drawOffset.x = flipX ? (-srcRect.left - spriteSize.width) : -srcRect.left;
		drawOffset.y = flipY ? (-srcRect.top - spriteSize.height) : -srcRect.top;

		// Render
		RenderSystem::Get().renderTarget->DrawImage(blurredImage.Get(), drawOffset);
		RenderSystem::Get().renderTarget->DrawImage(croppedImage.Get(), drawOffset);
	}
	else if (renderMode == RenderMode::Lit_ColorTint)
	{
		float pivotX = sprite->pivot.x;
		float pivotY = sprite->pivot.y;

		// === Crop ===
		cropEffect->SetInput(0, sprite->texture->texture2D.Get());
		cropEffect->SetValue(D2D1_CROP_PROP_RECT, srcRect);
		ComPtr<ID2D1Image> croppedImage;
		cropEffect->GetOutput(&croppedImage);

		// === Color Matrix ===
		colorMatrixEffect->SetInput(0, croppedImage.Get());
		colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
		ComPtr<ID2D1Image> colorImage;
		colorMatrixEffect->GetOutput(&colorImage);

		// === Blur Source ===
		blurEffect->SetInput(0, colorImage.Get());
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, blurAmmount);
		ComPtr<ID2D1Image> blurredImage;
		blurEffect->GetOutput(&blurredImage);

		// === Flip & Transform ===
		float scaleX = flipX ? -1.0f : 1.0f;
		float scaleY = flipY ? -1.0f : 1.0f;
		auto flipMat = D2D1::Matrix3x2F::Scale(D2D1::Size(scaleX, scaleY), { 0, 0 });

		D2D1_POINT_2F pivotOffset = { spriteSize.width * sprite->pivot.x, spriteSize.height * sprite->pivot.y };
		auto pivotMat = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
		auto finalMat = flipMat * pivotMat * transform->GetScreenMatrix();
		RenderSystem::Get().renderTarget->SetTransform(finalMat);

		// drawOffset 계산
		D2D1_POINT_2F drawOffset = { 0, 0 };
		drawOffset.x = flipX ? (-srcRect.left - spriteSize.width) : -srcRect.left;
		drawOffset.y = flipY ? (-srcRect.top - spriteSize.height) : -srcRect.top;

		// Render
		RenderSystem::Get().renderTarget->DrawImage(blurredImage.Get(), drawOffset);
		RenderSystem::Get().renderTarget->DrawImage(colorImage.Get(), drawOffset);
	}
}

// Set Color
void SpriteRenderer::SetColor(float r, float g, float b)
{
	// set RGBA
	colorMultiplier.r = r;
	colorMultiplier.g = g;
	colorMultiplier.b = b;

	// color maritx
	colorMatrix = {
	colorMultiplier.r, 0.0f,           0.0f,           0.0f,
	0.0f,           colorMultiplier.g, 0.0f,           0.0f,
	0.0f,           0.0f,           colorMultiplier.b, 0.0f,
	0.0f,           0.0f,           0.0f,           colorMultiplier.a
	};
}

// Set Alpha
void SpriteRenderer::SetAlpha(float a)
{
	// set alpha
	alpha = a;

	// set RGBA
	colorMultiplier.a = alpha;

	// color maritx
	colorMatrix = {
	colorMultiplier.r, 0.0f,           0.0f,           0.0f,
	0.0f,           colorMultiplier.g, 0.0f,           0.0f,
	0.0f,           0.0f,           colorMultiplier.b, 0.0f,
	0.0f,           0.0f,           0.0f,           colorMultiplier.a
	};
}