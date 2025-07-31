#include "ImageRenderer.h"
#include "RectTransform.h"
#include "GameObject.h"

void ImageRenderer::OnEnable_Inner()
{
    RenderSystem::Get().Regist(this);
    rectTransform = this->gameObject->rectTransform;

	// brush ����
	RenderSystem::Get().renderTarget->CreateSolidColorBrush(baseColor, brush.GetAddressOf());
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
}

void ImageRenderer::Update()
{

}

void ImageRenderer::Render()
{
    if (!rectTransform) return;

    // rect
    auto size = rectTransform->GetSize();
    destRect = { 0.0f, 0.0f, size.width, size.height };

    // tansform
    RenderSystem::Get().renderTarget->SetTransform(rectTransform->GetScreenMatrix());

    // render
    if (sprite)
    {
        RenderSystem::Get().renderTarget->DrawBitmap(
            sprite->texture->texture2D.Get(), 
            destRect, 
            alpha,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
        );
  
    }
    else
        RenderSystem::Get().renderTarget->FillRectangle(destRect, brush.Get());
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

// �̹����� ��� Brush�� �ٽ� �����ؾ��ϱ� ������ alpha�� �Ʒ� �Լ��� ���� �����ؾ��Ѵ�.
void ImageRenderer::SetAlpha(float a)
{
    alpha = a;
    if (brush) brush->SetOpacity(alpha);
}