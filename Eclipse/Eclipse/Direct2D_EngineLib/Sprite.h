#pragma once
#include <d2d1_1.h>
#include <string>
#include <iostream>
#include "Texture2D.h"

/* [Sprite]
* Texture2D의 Rect와 메타 데이터를 담은 그래픽 객체.
* Texuture 하나를 Sprite로 가질수도 있고, 일부 영역을 Sprite로 생성하여 비트맵 로드 비용을 줄일 수도 있다.
* ResourceManager에서 공유자원으로 관리되며 name이 key이기 때문에 생성할때 모두 다른 이름을 지정해주어야 한다.
* SpriteRenderer, ImageRenderer에서 렌더할 리소스로 사용되며 
* SpriteRenderer에서는 Sprite의 메타데이터대로 render되지만
* ImagerRenderer에서는 onwer 게임오브젝트의 RectTransform에 맞추어 렌더링된다.
*/

class Sprite
{
public:
	std::string name;                               // sprite name (리소스매니저의 키로 사용)
	D2D1_SIZE_F size = { 0, 0 };                    // sprite width, height
	D2D1_RECT_F sourceRect = { 0, 0, 0, 0 };        // texture에서 자를 영역
	D2D1_POINT_2F pivot = { 0.5f, 0.5f };           // {0,0} left, down
	std::shared_ptr<Texture2D> texture = nullptr;   // Texture2D

public:
    // texture 전체영역을 sprite로 생성
    Sprite(std::shared_ptr<Texture2D> tex, std::string spriteName)
    {
        texture = tex;
        name = spriteName;
        size = tex->size;
        sourceRect = { 0, 0, tex->size.width, tex->size.height };
    }

    // texture의 일부영역을 sprite로 생성
    Sprite(std::shared_ptr<Texture2D> tex, std::string spriteName, D2D1_RECT_F rect, D2D1_POINT_2F pivotPoint)
    {
        texture = tex;
        name = spriteName;
        sourceRect = rect;
        pivot = pivotPoint;

        size.width = rect.right - rect.left;
        size.height = rect.bottom - rect.top;

        // debug
        //OutputDebugStringA(("Sprite sourceRect: " + std::to_string(sourceRect.left) + ", " + std::to_string(sourceRect.top) + ", " + std::to_string(sourceRect.right) + ", " + std::to_string(sourceRect.bottom) + "\n").c_str());
        //OutputDebugStringA(("Sprite size: " + std::to_string(size.width) + "x" + std::to_string(size.height) + "\n").c_str());
    }
};