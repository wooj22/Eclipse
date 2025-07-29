#include "ScriptSystem.h"
#include "Script.h"

// component 등록
void ScriptSystem::Regist(Script* component)
{
	pending_components.push_back(component);
}

// component 등록 해제
void ScriptSystem::Unregist(Script* component)
{
	// script delete
	for (auto it = components.begin(); it != components.end(); ++it) {
		if (*it == component) {
			components.erase(it);
			return;
		}
	}

	// pending script delete
	for (auto it = pending_components.begin(); it != pending_components.end(); ++it) {
		if (*it == component) {
			pending_components.erase(it);
			return;
		}
	}
}

// update -> 프레임(deltaTime) 단위 호출
void ScriptSystem::Update()
{
	// pending_components push
	for (Script* script : pending_components)
	{
		components.push_back(script);
	}
	pending_components.clear();
	

	// update
	for (Script* script : components)
	{
		// update중 생성된 경우
		if (!script->started)
		{
			script->Awake(); 
			script->Start();
			script->started = true;
		}
		script->Update();
	}
}

// fixed udpate -> deltaTime에 따라 늦어졌다면 여러번 호출하고 넘어감
void ScriptSystem::FixedUpdate()
{
	// pending_components push
	for (Script* script : pending_components)
	{
		components.push_back(script);
	}
	pending_components.clear();

	// fixed udpate
	for (Script* script : components)
	{
		// update중 생성된 경우
		if (!script->started)
		{
			script->Awake();
			script->Start();
			script->started = true;
		}

		if (script) script->FixedUpdate();
	}
}