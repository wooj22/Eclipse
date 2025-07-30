#pragma once
#include <Windows.h>
#include "Object.h"

/* [Component]
* 게임 오브젝트에 부착될 컴포넌트들의 Base Class
*/

class GameObject;
class Component : public Object
{
public:
    GameObject* gameObject = nullptr;

private:
    bool enabled = true;

public:
    virtual ~Component() = default;

    /* Component Cycle */
    virtual void OnEnable_Inner() = 0;    // 컴포넌트 활성화시
	virtual void OnDisable_Inner() = 0;   // 컴포넌트 비활성화시
    virtual void OnDestroy_Inner() = 0;   // 컴포넌트 or 게임오브젝트 파괴시


    /* Enabled Func */
	bool IsEnabled() const { return enabled; }
	void SetEnabled(bool new_enable)
	{
		if (enabled == new_enable) return;
		enabled = new_enable;

		if (enabled)
			OnEnable_Inner();
		else
			OnDisable_Inner();
	}
};

