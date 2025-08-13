#include "CameraController.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"


CameraController::CameraController()
{
	mapRect.size = { 2560, 1920 };
}

void CameraController::Awake()
{
	camera = this->gameObject->GetComponent<Camera>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}


void CameraController::Start()
{
	// set target
	camera->SetTarget(playerTr);
	camera->SetTargetTraceXSpeed(450.0f);
	camera->SetTargetTraceYSpeed(800.0f);
	camera->SetTargetTraceLimitX(20.0f);
	camera->SetTargetTraceLimitY(30.0f);

	// set map rect
	camera->SetMapCondition(mapRect);
	camera->SetUseMapCondition(true);
}

void CameraController::LateUpdate()
{
	// test
	if(Input::GetKeyDown('I'))ZoomInToPlayer();
	if (Input::GetKeyDown('O'))ZoomOutFromPlayer();

	// zoom
	if (isZooming)
	{
		float delta = zoomSpeed * Time::GetDeltaTime();

		if (currentZoom < targetZoom) currentZoom = min(currentZoom + delta, targetZoom);
		else currentZoom = max(currentZoom - delta, targetZoom);
		camera->SetZoom(currentZoom);

		// end
		if (fabs(currentZoom - targetZoom) <= 0.001f)
		{
			currentZoom = targetZoom;
			isZooming = false;
		}
	}
}

void CameraController::ZoomInToPlayer()
{
	targetZoom = 2;
	isZooming = true;
	camera->SetTargetTraceLimitX(3.0f);
}

void CameraController::ZoomOutFromPlayer()
{
	targetZoom = 1;
	isZooming = true;
	camera->SetTargetTraceLimitX(20.0f);
}