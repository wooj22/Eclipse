#pragma once
#include <wrl/client.h> 
#include <d2d1_1.h>
#include <string>
#include "RenderSystem.h"
#include "IRenderer.h"

/* [World Text Renderer Conponent]
* <World>의 Text 출력을 담당하는 Component로
* 컴포넌트 생성시 RenderSystem에 등록되어 text를 계속 render한다.
* 게임 콘텐츠에서 text, font, size, color, layout을 지정할 수 있다. (dirty 패턴 활용)
* UI가 아닌 World의 Text로 GameObject의 Transform(position)만 참조하여 
* 본인만의 size(width, height)에 따라 center를 보정하여 중앙에 출력한다
* render시 필요한 transform은 이 컴포넌트가 등록된 오브젝트의 transform을 포인터에 담아 활용한다
*/

class Transform;
class WorldTextRenderer : public IRenderer
{
private:
	D2D1_POINT_2F centerPoint;	// text의 중심이 gameobect의 position으로 올 수 있게 하는 render point
	Transform* transform;

	// text
	std::wstring text = L"";
	float fontSize = 32.0f;
	std::wstring fontName = L"맑은 고딕";
	D2D1_COLOR_F textColor = D2D1::ColorF(D2D1::ColorF::White);
	int width = 100;
	int height = 100;

	// align
	TextHorizontalAlign horizontalAlign = TextHorizontalAlign::Center;
	TextVerticalAlign verticalAlign = TextVerticalAlign::Center;

	// d2d
	ComPtr<ID2D1SolidColorBrush> brush;
	ComPtr<IDWriteTextFormat> textFormat;
	ComPtr<IDWriteTextLayout> textLayout;

	bool isTextDirty;

public:
	// component cycle
	WorldTextRenderer() = default;
	~WorldTextRenderer() override = default;
	void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
	void Update() override final;
	void Render() override final;
	void OnDestroy_Inner() override final;

	// set
	void SetText(const std::wstring& newText);
	void SetFontSize(float newSize);
	void SetFontName(const std::wstring& newName);
	void SetColor(const D2D1_COLOR_F& newColor);
	void SetLayout(int width, int height);

	// align
	void SetHorizontalAlign(TextHorizontalAlign align);
	void SetVerticalAlign(TextVerticalAlign align);

private:
	// align
	DWRITE_TEXT_ALIGNMENT ToDWriteTextAlignment(TextHorizontalAlign align);
	DWRITE_PARAGRAPH_ALIGNMENT ToDWriteParagraphAlignment(TextVerticalAlign align);
};