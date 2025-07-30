#pragma once
#include <vector>
#include <queue>
#include <cstdint>
#include <unordered_set>
#include "Singleton.h"

/* [Object Table]
* 댕글링 포인터를 방지하기 위해 생성된 Object(GameObject, Component, Scene)의 생존여부를 관리하는 클래스
* 모든 Object는 생성시에 ObjectTable에 등록되고, 소멸시에 ObjectTable에서 제거된다.
*  ObjectTable::Get().IsValid(object) 함수를 통해 메모리에 살아있는 인스턴스인지 확인한 후 사용한다.
*/

class Object;
class ObjectTable : public Singleton<ObjectTable>
{
private:
	std::unordered_set<Object*> map_objects;

public:
	bool IsValid(Object* instance) 
	{
		return map_objects.find(instance) != map_objects.end();
	}

	void Register(Object* instance) 
	{
		map_objects.insert(instance);
	}

	void Unregister(Object* instance) 
	{
		map_objects.erase(instance);
	}
};

