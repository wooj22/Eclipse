#include "ImageRenderer.h"
#include "RectTransform.h"
#include "GameObject.h"

void ImageRenderer::OnEnable_Inner()
{
    RenderSystem::Get().Regist(this);
    rectTransform = this->gameObject->rectTransform;

	// brush 생성
	RenderSystem::Get().renderTarget->CreateSolidColorBrush(baseColor, brush.GetAddressOf());

    // effect 생성
    RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);
    RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1Crop, &cropEffect);
}

void ImageRenderer::OnDisable_Inner()
{
	RenderSystem::Get().Unregist(this);
	rectTransform = nullptr;
}

void ImageRenderer::OnDestroy_Inner()
{
    RenderSystem::Get().Unregist(this);
    sprite = nullptr;
    colorMatrixEffect = nullptr;
    cropEffect = nullptr;
}

void ImageRenderer::Update()
{

}

void ImageRenderer::Render()
{
    if (!rectTransform) return;

    auto size = rectTransform->GetSize();
    destRect = { 0.0f, 0.0f, size.width, size.height };

    auto& renderTarget = RenderSystem::Get().renderTarget;

    if (sprite)
    {
        // === Crop ===
        cropEffect->SetInput(0, sprite->texture->texture2D.Get());
        cropEffect->SetValue(D2D1_CROP_PROP_RECT, sprite->sourceRect);
        ComPtr<ID2D1Image> croppedImage;
        cropEffect->GetOutput(&croppedImage);

        // === Color Matrix ===
        colorMatrixEffect->SetInput(0, croppedImage.Get());
        colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, colorMatrix);
        ComPtr<ID2D1Image> finalImage;
        colorMatrixEffect->GetOutput(&finalImage);

        // === Transform ===
        // size에 맞게 채워질 수 있는 scale 수동 조정
        const auto& spriteSize = sprite->size;
        float scaleX = size.width / spriteSize.width;
        float scaleY = size.height / spriteSize.height;

        D2D1_MATRIX_3X2_F transform = rectTransform->GetScreenMatrix();
        D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(scaleX, scaleY);
        transform = scale * transform;

        // === Draw ===
        D2D1_MATRIX_3X2_F prevTransform;
        renderTarget->GetTransform(&prevTransform);
        renderTarget->SetTransform(transform);

        // render
        renderTarget->DrawImage(finalImage.Get(), nullptr);

        // 원래 transform으로 복원
        renderTarget->SetTransform(prevTransform);
    }
    else
    {
        renderTarget->SetTransform(rectTransform->GetScreenMatrix());
        renderTarget->FillRectangle(destRect, brush.Get());
    }
}

void ImageRenderer::SetBaseColor(const D2D1_COLOR_F& newColor) 
{
    baseColor = newColor;
    if (brush) {
        brush->SetColor(baseColor);
    }
    else {
        RenderSystem::Get().renderTarget->CreateSolidColorBrush(baseColor, brush.GetAddressOf());
    }
}

// 이미지의 경우 Brush를 다시 생성해야하기 때문에 alpha는 아래 함수를 통해 지정해야한다.
void ImageRenderer::SetAlpha(float a)
{
    alpha = a;
    if (brush) brush->SetOpacity(alpha);
}

// Set Color
void ImageRenderer::SetColor(float r, float g, float b)
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
    0.0f,           0.0f,           0.0f,           alpha
    };
}