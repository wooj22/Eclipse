#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <filesystem>
#include <iomanip>
#include <conio.h> 
#include <queue>

#include <d3d11.h>
#include <d2d1_3.h>
#include <wrl.h>
#include <wincodec.h>
#include <dxgi1_6.h>
#include <psapi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib,"windowscodecs.lib")

#include "Singleton.h"

using namespace std;
using namespace Microsoft::WRL;

/* [ResourceManager]
* 공유 자원을 중복으로 메모리에 로드하지 않도록 해시테이블로 관리하는 클래스
* key값으로 weak_ptr을 내부에서 관리를 하며, 외부 요청이 들어왔을 경우
* shared_ptr로 변환하여 리턴한다. (없을 경우에만 새로 생성)
*/

class Texture2D;
class Sprite;
class ResourceManager : public Singleton<ResourceManager>
{
private:
	unordered_map<string, weak_ptr<Texture2D>> map_texture2D;		// texture file path
	unordered_map<string, weak_ptr<Sprite>> map_sprite;				// sprite name
	//unordered_map<string, weak_ptr<AnimationClip>> map_animationClip;

private:
	// d2d
	ComPtr<IWICImagingFactory> wicImagingFactory;
	ComPtr<IDXGIDevice3> dxgiDevice; 
	ComPtr<IDXGIAdapter3> dxgiAdapter;
	
public:
	HRESULT Init();
	void UnInit();
	wstring FormatBytes(UINT64 bytes);
	wstring GetMemoryUsageWString();
	void PrintMemoryUsage();
	void Trim();

public:
	shared_ptr<Texture2D> CreateTexture2D(string filePath);
	shared_ptr<Sprite> CreateSprite(shared_ptr<Texture2D> texture, string spriteName);
	shared_ptr<Sprite> CreateSprite(shared_ptr<Texture2D> texture, string spriteName, D2D1_RECT_F rect, D2D1_POINT_2F pivotPoint = { 0.5f, 0.5f });
};
