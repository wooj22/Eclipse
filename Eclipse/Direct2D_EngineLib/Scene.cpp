#include "Scene.h"
#include "GameObject.h"

/// Scene Start
void Scene::Start()
{
	// pending objectList to objectList
	for (GameObject* object : pending_objectList)
	{
		objectList.push_back(object);
	}
	pending_objectList.clear();

	// start
	for (auto& object : objectList)
	{
		object->SceneStart();
	}
}

/// Scene Update
void Scene::Update()
{
	// pending objectList to objectList
	for (GameObject* object : pending_objectList)
	{
		objectList.push_back(object);
	}
	pending_objectList.clear();

	// game object update
	for (auto& object : objectList)
	{
		if (!object->IsDestroyed() && object->IsActive())	// ! capacity 재할당시 크래시 날 수 있음
			object->Update();
	}

	// destroyed gameobject delete
	auto it = objectList.begin();
	while (it != objectList.end()) 
	{
		GameObject* obj = *it;
		if (obj->IsDestroyed()) {
			obj->Destroyed();
			delete obj;
			it = objectList.erase(it);
		}
		else { ++it; }
	}
}

/// Scene Exit
void Scene::Exit()
{
	Clear();
}

/// Scene GameObject Clear
void Scene::Clear()
{
	// pending object delete
	for (auto& object : pending_objectList)
	{
		object->Destroyed();
		delete object;
	}
	pending_objectList.clear();

	// object delete
	for (auto& object : objectList)
	{
		object->Destroyed();
		delete object;
	}
	objectList.clear();
}

// Set GameObject List Capacity
// 메인 Play씬 같은 경우 capacity를 미리 할당받아 최적화
void Scene::SetObjectListCapacity(size_t capacity)
{
	objectList.reserve(capacity);
}