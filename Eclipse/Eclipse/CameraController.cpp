#include "CameraController.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Camera.h"


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
	camera->SetTargetTraceYSpeed(1500.0f);
	camera->SetTargetTraceLimitX(20.0f);
	camera->SetTargetTraceLimitY(30.0f);

	// set map rect
	camera->SetMapCondition(mapRect);
}

void CameraController::Update()
{

}