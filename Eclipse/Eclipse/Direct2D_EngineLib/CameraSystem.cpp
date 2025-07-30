#include "CameraSystem.h"
#include "Camera.h"

/// Component 등록
void CameraSystem::Regist(Camera* component)
{
	pending_components.push_back(component);
}

/// Component 등록 해제
void CameraSystem::Unregist(Camera* component)
{
	// delete
	for (auto it = components.begin(); it != components.end(); ++it) {
		if (*it == component) {
			components.erase(it);
			return;
		}
	}

	// pending delete
	for (auto it = pending_components.begin(); it != pending_components.end(); ++it) {
		if (*it == component) {
			pending_components.erase(it);
			return;
		}
	}
}

void CameraSystem::Update()
{
	// pending_components push
	for (Camera* camera : pending_components)
	{
		components.push_back(camera);
	}
	pending_components.clear();


	// update
	for (Camera* camera : components)
	{
		camera->Update();
	}
}