#include "ButtonSystem.h"
#include "Button.h"

// component 등록
void ButtonSystem::Regist(Button* component)
{
	pending_components.push_back(component);
}

// component 등록 해제
void ButtonSystem::Unregist(Button* component)
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
void ButtonSystem::Update()
{
	// pending_components push
	for (Button* btn : pending_components)
	{
		components.push_back(btn);
	}
	pending_components.clear();


	// update
	for (Button* btn : components)
	{
		btn->Update();
	}
}