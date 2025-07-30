#include "AnimatorSystem.h"
#include "Animator.h"

// component 등록
void AnimatorSystem::Regist(Animator* component)
{
	pending_components.push_back(component);
}

// component 등록 해제
void AnimatorSystem::Unregist(Animator* component)
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
void AnimatorSystem::Update()
{
	// pending_components push
	for (Animator* animator : pending_components)
	{
		components.push_back(animator);
	}
	pending_components.clear();


	// update
	for (Animator* animator : components)
	{
		animator->Update();
	}
}
