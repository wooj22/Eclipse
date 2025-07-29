#pragma once
#include <vector>
#include <windows.h> 
#include "GameObject.h"
#include "Object.h"
#include "Transform.h"

using namespace std;

/* [Scene]
* 게임오브젝트으 컨테이너 역할을 수행하며 게임 오브젝트 사이클을 호출한다.
* 씬에 GameObject가 생성될 때 : GameObject->Awake()
* 씬이 Start될 때 : GameObject->SceneStartInit()
* 씬이 Update될 때 : GameObject->Update()
* 씬의 GameObject를 삭제시킬 때 : GameObject->Destroyed()
* 
* 게임오브젝트가 생성될때 바로 objectList에 추가하는게 아닌, 
* pending_objectList에 추가하고, vector의 update가 돌기 전 objectList로 추가한다.
* Update중 capacity가 변경될 경우 크래시를 방지할 수 있다.
*/

class GameObject;
class Scene : public Object
{
private:
	vector<GameObject*> objectList;
	vector<GameObject*> pending_objectList;

public:
	Scene() = default;
	virtual ~Scene() { Clear(); }

	/* Scene Life Cycle */
	virtual void Awake() = 0;
	virtual void Start();		// GameObjects->SceneStartInit()
	virtual void Update();		// GameObjects->Update()
	virtual void Exit();		// GameObjects->Destroyed()
	void Clear();

	// Create
	template<typename T, typename... Args>
	T* CreateObject(Vector2 position = {0,0}, GameObject* parent = nullptr, Args&&... args)
	{
		// new
		T* pObject = new T(std::forward<Args>(args)...);
		pending_objectList.push_back(pObject);

		// transform, parent
		Transform* tr = pObject->GetComponent<Transform>();
		if (tr)
		{
			tr->SetPosition(position);
			if (parent)
			{
				Transform* parentTr = parent->GetComponent<Transform>();
				if (parentTr)
				{
					tr->SetParent(parentTr);
				}
			}
		}

		// cycle
		pObject->Awake();
		return pObject;
	}

	// Delete
	template<typename T>
	void DeleteObject(T object)
	{
		auto it = find(objectList.begin(), objectList.end(), object);
		if (it != objectList.end()) {
			(*it)->Destroyed();;
			delete *it;
			objectList.erase(it);
		}

		else {
			OutputDebugStringA("삭제할 GameObject가 없습니다.\n");
		}
	}

	// Set capacity
	void SetObjectListCapacity(size_t capacity);
};

