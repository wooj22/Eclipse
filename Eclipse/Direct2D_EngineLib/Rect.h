#pragma once
#include "Vector2.h"

class Rect
{
public:
    Vector2 position;
    Vector2 size;

    float Left() const { return position.x - size.x / 2; }
    float Right() const { return position.x + size.x / 2; }
    float Bottom() const { return position.y - size.y / 2; }
    float Top() const { return position.y + size.y / 2; }
};