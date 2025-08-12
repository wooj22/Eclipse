#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Rect.h"

// InGameCamera Controll Script

class Transform;
class Camera;

class CameraController : public Script
{
private:
	// zoom data
	float currentZoom = 1.0f;
	float targetZoom = 1.0f;
	float zoomSpeed = 2.0f;
	bool isZooming = false;

	// map rect
	Rect mapRect;

	// ref
	Camera* camera;
	Transform* playerTr;

public:
	CameraController();

	void Awake() override;
	void Start() override;
	void LateUpdate() override;

public:
	// func
	void ZoomInToPlayer();
	void ZoomOutFromPlayer();
};

