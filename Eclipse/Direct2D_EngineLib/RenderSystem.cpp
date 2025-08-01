#include "RenderSystem.h"
#include "SpriteRenderer.h"
#include "WorldTextRenderer.h"
#include "ImageRenderer.h"
#include "ScreenTextRenderer.h"
#include "ColliderSystem.h"
#include "Camera.h"
#include <algorithm>

/// Component 등록
void RenderSystem::Regist(IRenderer* component)
{
	if (component->rendertype == RenderType::GameObject)
	{
		pending_game_renderers.push_back(component);
	}
	else if (component->rendertype == RenderType::UI)
	{
		pending_ui_renderers.push_back(component);
	}
}

/// Component 등록 해제
void RenderSystem::Unregist(IRenderer* component)
{
	// delete
	for (auto it = game_renderers.begin(); it != game_renderers.end(); ++it) {
		if (*it == component) {
			game_renderers.erase(it);
			return;
		}
	}
	for (auto it = ui_renderers.begin(); it != ui_renderers.end(); ++it) {
		if (*it == component) {
			ui_renderers.erase(it);
			return;
		}
	}

	// pending delete
	for (auto it = pending_game_renderers.begin(); it != pending_game_renderers.end(); ++it) {
		if (*it == component) {
			pending_game_renderers.erase(it);
			return;
		}
	}
	for (auto it = pending_ui_renderers.begin(); it != pending_ui_renderers.end(); ++it) {
		if (*it == component) {
			pending_ui_renderers.erase(it);
			return;
		}
	}
}

/// Init
void RenderSystem::Init(HWND hwnd, int width, int height) 
{
	this->hwnd = hwnd;
	this->width = width;
	this->height = height;

	// D3D11 Device
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D Factory, Device
	ComPtr<ID2D1Factory8> d2dFactory;		// D2D Factory : Direct2D 객체를 생성하는 팩토리
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;			// DXGI Device
	d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;			// D2D Device
	d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());

	// D2D Device context
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, renderTarget.GetAddressOf());

	// DXGI Factory
	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;
	scDesc.Height = height;
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiFactory->CreateSwapChainForHwnd(d3dDevice.Get(), hwnd, &scDesc, nullptr, nullptr, swapChain.GetAddressOf());

	// 백버퍼를 타겟으로 설정 -> DeginDraw(), EndDraw() 가능
	ComPtr<IDXGISurface> backBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(		// 버퍼를 D2D용 Bitmap으로 감쌈
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	renderTarget->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, backBufferBitmap.GetAddressOf());
	renderTarget->SetTarget(backBufferBitmap.Get());

	// DirectWrite 팩터리를 만듭니다.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(dWriteFactory.GetAddressOf()));

	// brush
	renderTarget->CreateSolidColorBrush(debug_color, &debug_brush);
	renderTarget->CreateSolidColorBrush(line_color, &line_brush);
}

void RenderSystem::Update()
{
	// pending components push
	for (IRenderer* renderer : pending_game_renderers)
	{
		game_renderers.push_back(renderer);
	}
	pending_game_renderers.clear();
	for (IRenderer* renderer : pending_ui_renderers)
	{
		ui_renderers.push_back(renderer);
	}
	pending_ui_renderers.clear();


	// GameObject Update()
	for (IRenderer* renderer : game_renderers)
	{
		renderer->Update();
	}

	// UI Update()
	for (IRenderer* renderer : ui_renderers)
	{
		renderer->Update();
	}
}

/// Render
void RenderSystem::Render()
{
	// pending components push
	for (IRenderer* renderer : pending_game_renderers)
	{
		game_renderers.push_back(renderer);
	}
	pending_game_renderers.clear();
	for (IRenderer* renderer : pending_ui_renderers)
	{
		ui_renderers.push_back(renderer);
	}
	pending_ui_renderers.clear();


	/*         render          */
	renderTarget->BeginDraw();
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// layer sort
	sort(game_renderers.begin(), game_renderers.end(),
		[](IRenderer* a, IRenderer* b) { return a->layer < b->layer; });

	sort(ui_renderers.begin(), ui_renderers.end(),
		[](IRenderer* a, IRenderer* b) { return a->layer < b->layer; });

	// GameObject Render()
	// culling
	for (IRenderer* renderer : game_renderers)
	{
		if (Camera::GetMainCamera()->IsInView(renderer->boundPos, renderer->boundSize))
		{
			renderer->Render();
		}
	}

	// UI Render()
	for (IRenderer* renderer : ui_renderers)
	{
		renderer->Render();
	}

	// debug draw
	for (const DebugDrawCommand& cmd : debugDrawCommands)
	{
		renderTarget->SetTransform(cmd.transform);
		switch (cmd.type)
		{
		case DebugDrawType::Rect:
			renderTarget->DrawRectangle(cmd.rect, debug_brush.Get(), cmd.strokeWidth);
			break;
		case DebugDrawType::Circle:
			renderTarget->DrawEllipse(cmd.ellipse, debug_brush.Get(), cmd.strokeWidth);
			break;
		case DebugDrawType::Line:
			renderTarget->DrawLine(cmd.line.start, cmd.line.end, line_brush.Get(), cmd.strokeWidth);
			break;
		}
	}
	debugDrawCommands.clear();

	renderTarget->EndDraw();
	swapChain->Present(1, 0);
}

/// UnInit
void RenderSystem::UnInit()
{
	// d2d
	d3dDevice.Reset();
	swapChain.Reset();
	renderTarget.Reset();
	backBufferBitmap.Reset();
	dWriteFactory.Reset();

	line_brush.Reset();
	debug_brush.Reset();
}


// debug
// transform : GetScreenMatrix()으로 넘겨주어야함
void RenderSystem::DebugDrawRect(const D2D1_RECT_F& rect, const D2D1_MATRIX_3X2_F& transform, float strokeWidth)
{
	DebugDrawCommand cmd;
	cmd.type = DebugDrawType::Rect;
	cmd.rect = rect;
	cmd.transform = transform;
	cmd.strokeWidth = strokeWidth;
	debugDrawCommands.push_back(cmd);
}

void RenderSystem::DebugDrawCircle(const D2D1_ELLIPSE& ellipse, const D2D1_MATRIX_3X2_F& transform, float strokeWidth)
{
	DebugDrawCommand cmd;
	cmd.type = DebugDrawType::Circle;
	cmd.ellipse = ellipse;
	cmd.transform = transform;
	cmd.strokeWidth = strokeWidth;
	debugDrawCommands.push_back(cmd);
}

void RenderSystem::DebugDrawLine(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, const D2D1_MATRIX_3X2_F& transform, float strokeWidth)
{
	DebugDrawCommand cmd;
	cmd.type = DebugDrawType::Line;
	cmd.line = { D2D1::Point2F(start.x, -start.y), D2D1::Point2F(end.x, -end.y) };		// y 반전
	cmd.transform = transform;
	cmd.strokeWidth = strokeWidth;
	debugDrawCommands.push_back(cmd);
}