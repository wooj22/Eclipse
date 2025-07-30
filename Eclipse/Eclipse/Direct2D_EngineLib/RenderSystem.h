#pragma once
#include <assert.h>
#include <windows.h>     // Win32 API
#include <wrl.h>         // ComPtr 스마트 포인터
#include <d3d11.h>       // Direct3D
#include <d2d1_3.h>      // Direct2D
#include <dxgi1_6.h>     // DXGI
#include <wincodec.h>    // WIC 
#include <dwrite.h>	     // Write
#include "Singleton.h"
#include "IRenderer.h"
#include "DebugDrawCommand.h"
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"dwrite.lib")
using namespace Microsoft::WRL;    // Microsoft::WRL::ComPtr<T>
using namespace std;


/* RenderSystem의 역할
* 1. render를 위한 directX 기반 마련 -> 모든 render는 이곳에서 이루어져야함
* 2. Renderer 컴포넌트들의 update, render
* 3. Debug Utility를 제공한다.
*/

class RenderSystem : public Singleton<RenderSystem>
{
private:
	vector<IRenderer*> ui_renderers;
	vector<IRenderer*> game_renderers;

	vector<IRenderer*> pending_ui_renderers;
	vector<IRenderer*> pending_game_renderers;

	// window
	HWND hwnd;
	int width;
	int height;

public :
	// DirectX 2D Base
	ComPtr<ID3D11Device> d3dDevice;				    // D3D Device (gpu 연결)
	ComPtr<IDXGISwapChain1> swapChain;	            // SwapChain (더블버퍼링)
	ComPtr<ID2D1DeviceContext7> renderTarget;       // D2D Device Context (그리기)
	ComPtr<ID2D1Bitmap1> backBufferBitmap;	  	    // 화면 출력용 D2D Bitmap (그릴 대상)
	ComPtr<IDWriteFactory> dWriteFactory;			// Text Write Factory (텍스트 그리기)

public:
	// componenet
	void Regist(IRenderer* component);
	void Unregist(IRenderer* component);

	// component system
	void Init(HWND hwnd, int width, int height);
	void Update();
	void Render();
	void UnInit();			


private:
	// debug
	vector<DebugDrawCommand> debugDrawCommands;
	ComPtr<ID2D1SolidColorBrush> debug_brush;
	D2D1_COLOR_F debug_color = D2D1::ColorF(D2D1::ColorF::Red);

public:
	// 함수를 호출하면 vector에 저장되어 해당 프레임에 그려지고 clear 된다.
	// 그려질 rect, circle, line과 ScreenMatrix를 전달해주면 된다.
	void DebugDrawRect(const D2D1_RECT_F& rect, const D2D1_MATRIX_3X2_F& transform, float strokeWidth = 0.5f);
	void DebugDrawCircle(const D2D1_ELLIPSE& ellipse, const D2D1_MATRIX_3X2_F& transform, float strokeWidth = 0.5f);
	void DebugDrawLine(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, const D2D1_MATRIX_3X2_F& transform, float strokeWidth = 0.5f);
};
