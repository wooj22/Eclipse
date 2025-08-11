#pragma once
#include <wrl/client.h> 
#include <d2d1_1.h>
#include <string>
#include "RenderSystem.h"
#include "IRenderer.h"
#include "FontLoaderUtil.h"

/* [Screen Text Renderer Conponent]
* <UI>의 Text 출력을 담당하는 Component로
* 컴포넌트 생성시 RenderSystem에 등록되어 text를 계속 render한다.
* 게임 콘텐츠에서 text, font, size, color를 지정할 수 있다. (dirty 패턴 활용)
* RectTransform의 Rect영역의 중앙에 출력하며 size(widht, height) 또한 rectTransform에 제한된다.
* render시 필요한 RectTransform은 이 컴포넌트가 등록된 오브젝트의 RectTransform의 포인터에 담아 활용한다
*/

class FontLoaderUtil;
class RectTransform;
class ScreenTextRenderer : public IRenderer
{
private:
	//D2D1_POINT_2F centerPoint;
	RectTransform* rectTransform;

	// text
	std::wstring text = L"";
	float fontSize = 18.0f;
	std::wstring fontName = L"Noto Serif KR";
	D2D1_COLOR_F textColor = D2D1::ColorF(D2D1::ColorF::White);
	float alpha = 1.0f;

	// align
	TextHorizontalAlign horizontalAlign = TextHorizontalAlign::Center;
	TextVerticalAlign verticalAlign = TextVerticalAlign::Center;

	// d2d
	ComPtr<ID2D1SolidColorBrush> brush;
	ComPtr<IDWriteTextFormat> textFormat;
	ComPtr<IDWriteTextLayout> textLayout;

	bool isTextDirty;
	FontLoaderUtil fontUtil;

public:
	// component cycle
	ScreenTextRenderer()
	{ 
		rendertype = RenderType::UI;		// 렌더 타입 지정 (UI)
	};
	~ScreenTextRenderer() override = default;

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
	void SetAlpha(float a);

	// align
	void SetHorizontalAlign(TextHorizontalAlign align);
	void SetVerticalAlign(TextVerticalAlign align);

private:
	// align
	DWRITE_TEXT_ALIGNMENT ToDWriteTextAlignment(TextHorizontalAlign align);
	DWRITE_PARAGRAPH_ALIGNMENT ToDWriteParagraphAlignment(TextVerticalAlign align);
};

