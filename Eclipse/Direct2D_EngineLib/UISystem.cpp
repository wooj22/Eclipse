#include "UISystem.h"
#include "I_UI.h"

// component 등록
void UISystem::Regist(I_UI* component)
{
	pending_components.push_back(component);
}

// component 등록 해제
void UISystem::Unregist(I_UI* component)
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
void UISystem::Update()
{
	// pending_components push
	for (I_UI* ui : pending_components)
	{
		components.push_back(ui);
	}
	pending_components.clear();


	// update
	for (I_UI* ui : components)
	{
		ui->Update();
	}
}