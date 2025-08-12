#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Rect.h"

// InGameCamera Controll Script

class Transform;
class Camera;

class CameraController : public Script
{
private:
	// map rect
	Rect mapRect;

	// ref
	Camera* camera;

	// player
	Transform* playerTr;

public:
	CameraController();

	void Awake() override;
	void Start() override;
	void Update() override;
};

