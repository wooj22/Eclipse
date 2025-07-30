#pragma once
#include "ObjectTable.h"

/* [Object]
* Scene, GameObject, Component의 기본 클래스로
* 생성될 떄 ObjectTable에 생성된 메모리 주소를 등록하고, 메모리에서 소멸될 때 삭제하여
* 인스턴스가 현재 메모리에 살아있는지 확인할 수 있도록 한다.
* GameObject 참조, Component 참조, 콜백함수 호출 등에서 댕글링 포인터 문제를 방지한다.
* ObjectTable::Get().IsValid(object)
*/

class Object
{
public:
	// 유효성 검사
	Object() { ObjectTable::Get().Register(this); }
	virtual ~Object() { ObjectTable::Get().Unregister(this); }
};

