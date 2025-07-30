#pragma once
#include <vector>
#include "Singleton.h"
using namespace std;

/* ButtonSystem의 역할
* 1. Button Component update 
     -> 자신의 영역이 클릭되어있는지 감지하고 이벤트 발생시 콜백 함수 invoke
*/

class Button;
class ButtonSystem : public Singleton<ButtonSystem>
{
private:
	vector<Button*> components;
	vector<Button*> pending_components;

public:
	// componenet
	void Regist(Button* component);
	void Unregist(Button* component);

	// component system
	void Update();
};

