#pragma once
#include "Component.h"
#include "Vector2.h"

/* Text Aling */
enum class TextHorizontalAlign {
	Left,
	Center,
	Right
};

enum class TextVerticalAlign {
	Top,
	Center,
	Bottom
};

/* Color Matrix member */
struct ColorRGBA
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;
};

/* Render Type */
// RenderSystem Render :: GameObject -> UI
enum class RenderType
{
	UI, GameObject
};


/* Render Mode */
// Render 분기 처리
enum class RenderMode
{
	Unlit,					// Draw Bitmap
	UnlitColorTint,			// Draw Image (Crop + ColorEffect)
	Lit_Glow,				// Draw Image (Crop + BlurEffect)
	Lit_ColorTint,			// Draw Image (Crop + ColorEffect + BlurEffect)
};

/* --------------------------------*/
/* Renderer Componenets Interface */
/* ------------------------------*/
class IRenderer : public Component
{
public :  
	RenderType rendertype = RenderType::GameObject;
	RenderMode renderMode = RenderMode::Unlit;		// sprite (SprteRenderer, ImageRenderer)
	int layer = 0;

	// culling - gameobject
	Vector2 boundPos;   // world pos + pivot
	Vector2 boundSize;  // size

	// component cycle
	virtual ~IRenderer() = default;
	virtual void Update() = 0;
	virtual void Render() = 0;
};