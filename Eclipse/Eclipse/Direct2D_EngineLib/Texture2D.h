#pragma once
#include <wrl/client.h> 
#include <d2d1_1.h>
#include <string>

/* [Texture2D]
* 2D bitmap 리소스 하나를 가지는 클래스
* Sprite의 멤버로 등록되어 SpriteRenderer, ImageRenderer에서 render 대상 객체로 사용된다.
* Texture는 하나의 Sprite가 될 수도 있고, 여러 Sprite로 쪼개질 수도 있다.
* ResourceManager에서 관리되어 여러 Sprite가 각각의 Textrue를 가지지 않고 자원을 공유할 수 있게끔 한다.
*/

class Texture2D
{
public:
	std::string filePath;								// file path
	D2D1_SIZE_F size;									// width, height
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> texture2D;		// bitmap image
};