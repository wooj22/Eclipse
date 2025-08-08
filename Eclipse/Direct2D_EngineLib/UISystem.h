#pragma once
#include <vector>
#include "Singleton.h"
using namespace std;

/* UISystem의 역할
* 1. 각 UI 컴포넌트들의 Update 호출 (button, slider 등)
*/

class I_UI;
class UISystem : public Singleton<UISystem>
{
private:
	vector<I_UI*> components;
	vector<I_UI*> pending_components;

public:
	// componenet
	void Regist(I_UI* component);
	void Unregist(I_UI* component);

	// component system
	void Update();
};

