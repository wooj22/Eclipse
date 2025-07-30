#include "PhysicsSystem.h"
#include "Rigidbody.h"

// component 등록
void PhysicsSystem::Regist(Rigidbody* component)
{
	pending_components.push_back(component);
}

// component 등록 해제
void PhysicsSystem::Unregist(Rigidbody* component)
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
void PhysicsSystem::FixedUpdate()
{
	// pending_components push
	for (Rigidbody* rb : pending_components)
	{
		components.push_back(rb);
	}
	pending_components.clear();

	// fixed udpate
	for (Rigidbody* rb : components)
	{
		if (rb) rb->FixedUpdate();
	}
}
