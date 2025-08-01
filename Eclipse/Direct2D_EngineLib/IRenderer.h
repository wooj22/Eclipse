#pragma once
#include "Component.h"
#include "Vector2.h"

/* Renderer Componenets Interface */

enum class RenderType
{
	UI, GameObject
};

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

struct ColorRGBA
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;
};

class IRenderer : public Component
{
public : 
	// layer
	RenderType rendertype = RenderType::GameObject;
	int layer = 0;

	// culling - gameobject
	Vector2 boundPos;   // world pos + pivot
	Vector2 boundSize;  // size

	// component cycle
	virtual ~IRenderer() = default;
	virtual void Update() = 0;
	virtual void Render() = 0;
};