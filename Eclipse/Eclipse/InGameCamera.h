#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "CameraController.h"

/* PLAY씬에서 활용될 MainCamera */

class InGameCamera : public GameObject
{
private:
	Transform* transform;
	Camera* camera;
	CameraController* controller;

public:
	InGameCamera() : GameObject("InGameCamera","InGameCamera")
	{
		transform = AddComponent<Transform>();
		camera = AddComponent<Camera>(1920, 1080);
		controller = AddComponent<CameraController>();
		Camera::SetMainCamera(camera);
	}
};

