#include "ParallaxLayer.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Camera.h"

void ParallaxLayer::Awake()
{
	tr = this->gameObject->transform;
	maintCameraTr = Camera::GetMainCamera()->gameObject->transform;
	privousCameraX = maintCameraTr->GetWorldPosition().x;
}

void ParallaxLayer::Update()
{
	currentCameraX = maintCameraTr->GetWorldPosition().x;
	float deltaX = currentCameraX - privousCameraX;
	tr->Translate(deltaX * parallaxFactor, 0);

	privousCameraX = currentCameraX;
}