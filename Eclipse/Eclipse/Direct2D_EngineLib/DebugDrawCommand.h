#pragma once
#include <d2d1.h>

// debug type
enum class DebugDrawType
{
	Rect, Circle, Line
};

// debug draw command
// render system의 debug draw를 저장하는 객체
struct DebugDrawCommand
{
	DebugDrawType type;					// draw type
	D2D1_MATRIX_3X2_F transform;		// transform
	float strokeWidth;					// 선 굵기

	// union : 여러 멤버가 같은 메모리 공간을 공유
	union
	{
		D2D1_RECT_F rect;
		D2D1_ELLIPSE ellipse;
		struct { D2D1_POINT_2F start, end; } line;
	};
};