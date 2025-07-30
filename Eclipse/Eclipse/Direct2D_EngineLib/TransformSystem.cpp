#include "TransformSystem.h"
#include "Transform.h"
#include "RectTransform.h"

// component 등록
void TransformSystem::Regist(ITransform* component)
{
	pending_components.push_back(component);
}

// component 등록 해제
void TransformSystem::Unregist(ITransform* component)
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

// component system
void TransformSystem::Update() 
{
	// pending_components push
	for (ITransform* component : pending_components)
	{
		components.push_back(component);
	}
	pending_components.clear();


	// update
	for (ITransform* component : components)
	{
		component->Update();
	}
}