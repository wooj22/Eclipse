#pragma once
#include "Component.h"
#include "MultiDelegate.h"
#include "ButtonSystem.h"

/* [Button Conponent]
* <UI>의 일정 영역의 OnClick() 이벤트를 감시하고 발생시키하는 컴포넌트
* 이벤트 발생시 onClickListeners에 등록된 콜백 함수를 모두 호출시킨다.
* RectTransform을 포함한 GameObject에 등록되어야 정상적인 Button의 기능을 활용할 수 있다.
*/

class RectTransform;
class Button : public Component
{
private:
    RectTransform* rectTransform; 

public:
    // component cycle
    Button() = default;
    ~Button() override = default;

    void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
    void Update();
    void OnDestroy_Inner() override final;

private:
    bool isMouseInside = false;

public:
    /* Event */
    MultiDelegate<> onClickListeners;       // On Click
    MultiDelegate<> onPointEnterListeners;  // On Point Enter
    MultiDelegate<> onPointExitListeners;   // On Point Exit

private:
    void OnClick();
    void OnPointEnter();
    void OnPointExit();
};

