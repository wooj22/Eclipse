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
    RenderSystem::Get().renderTarget->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect);
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
    blurEffect = nullptr;
}

void ImageRenderer::Update()
{

}

void ImageRenderer::Render()
{
    if (!rectTransform) return;

    // rect
    auto size = rectTransform->GetSize();
    D2D1_RECT_F destRect = { 0.0f, 0.0f, size.width, size.height };

    // render targt get
    auto& renderTarget = RenderSystem::Get().renderTarget;

    if (sprite)
    {
        if (renderMode == RenderMode::Unlit)
        {
            // tansform
            RenderSystem::Get().renderTarget->SetTransform(rectTransform->GetScreenMatrix());

            // render
            if (sprite)
            {
                D2D1_RECT_F srcRect = sprite->sourceRect;

                // fill type
                D2D1_RECT_F adjustedSrcRect = srcRect;
                D2D1_RECT_F adjustedDstRect = destRect;

                switch (fillType)
                {
                case FillType::Horizontal:
                    adjustedSrcRect.right = srcRect.left + (srcRect.right - srcRect.left) * fillAmount;
                    adjustedDstRect.right = destRect.left + (destRect.right - destRect.left) * fillAmount;
                    break;

                case FillType::Vertical:
                    adjustedSrcRect.top = srcRect.bottom - (srcRect.bottom - srcRect.top) * fillAmount;
                    adjustedDstRect.top = destRect.bottom - (destRect.bottom - destRect.top) * fillAmount;
                    break;

                case FillType::None:
                    break;
                default:
                    break;
                }

                RenderSystem::Get().renderTarget->DrawBitmap(
                    sprite->texture->texture2D.Get(),
                    adjustedDstRect,
                    colorMultiplier.a,
                    D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                    adjustedSrcRect
                );
            }
            else
                RenderSystem::Get().renderTarget->FillRectangle(destRect, brush.Get());
        }
        else if (renderMode == RenderMode::UnlitColorTint)
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
        else if (renderMode == RenderMode::Lit_Glow)
        {
            // === Crop ===
            cropEffect->SetInput(0, sprite->texture->texture2D.Get());
            cropEffect->SetValue(D2D1_CROP_PROP_RECT, sprite->sourceRect);
            ComPtr<ID2D1Image> croppedImage;
            cropEffect->GetOutput(&croppedImage);

            // === Blur Source ===
            blurEffect->SetInput(0, croppedImage.Get());
            blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, blurAmmount);
            ComPtr<ID2D1Image> blurredImage;
            blurEffect->GetOutput(&blurredImage);

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

            // Render
            RenderSystem::Get().renderTarget->DrawImage(blurredImage.Get(), nullptr);
            RenderSystem::Get().renderTarget->DrawImage(croppedImage.Get(), nullptr);

            // 원래 transform으로 복원
            renderTarget->SetTransform(prevTransform);
        }
        else if (renderMode == RenderMode::Lit_ColorTint)
        {
            // === Crop ===
            cropEffect->SetInput(0, sprite->texture->texture2D.Get());
            cropEffect->SetValue(D2D1_CROP_PROP_RECT, sprite->sourceRect);
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

            // Render
            RenderSystem::Get().renderTarget->DrawImage(blurredImage.Get(), nullptr);
            RenderSystem::Get().renderTarget->DrawImage(colorImage.Get(), nullptr);

            // 원래 transform으로 복원
            renderTarget->SetTransform(prevTransform);
        }
    }
    else
    {
        renderTarget->SetTransform(rectTransform->GetScreenMatrix());
        renderTarget->FillRectangle(destRect, brush.Get());
    }
}

// Set Image Color
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

// Set Sprite Color
void ImageRenderer::SetColor(float r, float g, float b)
{
    colorMultiplier.r = r;
    colorMultiplier.g = g;
    colorMultiplier.b = b;

    // color Matrix
    colorMatrix._11 = colorMultiplier.r;  // Red
    colorMatrix._22 = colorMultiplier.g;  // Green
    colorMatrix._33 = colorMultiplier.b;  // Blue
}

// Set Aplha (sprite & image)
void ImageRenderer::SetAlpha(float a)
{
    colorMultiplier.a = a;

    // brush
    if (brush) brush->SetOpacity(colorMultiplier.a);

    // color maritx
    colorMatrix._44 = colorMultiplier.a;  // Alpha
}

// Saturation
void ImageRenderer::SetSaturation(float s)
{
    saturation = s;

    // color matrix
    colorMatrix = {
        rw + (1 - rw) * saturation, gw - gw * saturation,     bw - bw * saturation,     0,
        rw - rw * saturation,       gw + (1 - gw) * saturation, bw - bw * saturation,   0,
        rw - rw * saturation,       gw - gw * saturation,     bw + (1 - bw) * saturation, 0,
        0,                 0,               0,               colorMultiplier.a
    };
}

// Set Brightness
void ImageRenderer::SetBrightness(float scale)
{
    brightnessScale = scale;

    // color matrix
    colorMatrix = {
        brightnessScale, 0,                0,                0,
        0,               brightnessScale,  0,                0,
        0,               0,                brightnessScale,  0,
        0,               0,                0,                colorMultiplier.a
    };
}