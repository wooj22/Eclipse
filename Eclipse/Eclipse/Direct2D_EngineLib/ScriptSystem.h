#pragma once
#include <vector>
#include "Singleton.h"
using namespace std;

/* ScriptSystem의 역할
* 1. Script 컴포넌트의 life cycle함수를 관리한다.
*   - 충돌 이벤트 함수는 collider쪽에서 호출됨
*/

class Script;
class ScriptSystem : public Singleton<ScriptSystem>
{
private:	
	vector<Script*> components;
	vector<Script*> pending_components;

public:
	// componenet
	void Regist(Script* component);
	void Unregist(Script* component);

	// component system
	void Update();
	void FixedUpdate();
};