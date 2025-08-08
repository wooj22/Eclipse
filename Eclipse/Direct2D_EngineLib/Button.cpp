#include "Button.h"
#include "RectTransform.h"
#include "ImageRenderer.h"
#include "GameObject.h"
#include "Input.h"
#include <iostream>
#include <string>

void Button::OnEnable_Inner()
{
    UISystem::Get().Regist(this);
    rectTransform = this->gameObject->rectTransform;
}

void Button::OnDisable_Inner()
{
	UISystem::Get().Unregist(this);
	rectTransform = nullptr;
}

void Button::OnDestroy_Inner()
{
    UISystem::Get().Unregist(this);
}

void Button::Update() 
{
    if (!rectTransform) return;

    // mouse position
    Vector2 mouse = Input::GetMouseScreenPosition();
    float mouseX = mouse.x;
    float mouseY = mouse.y;

    // button rect
    Vector2 pos = rectTransform->GetWorldPosition();
    D2D1_SIZE_F size = rectTransform->GetSize();
    D2D1_POINT_2F pivot = rectTransform->GetPivot();

    float left = pos.x - (size.width * pivot.x);
    float top = pos.y - (size.height * pivot.y);
    float right = left + size.width;
    float bottom = top + size.height;

    // mouse inside button?
    bool inside = (mouseX >= left && mouseX <= right &&
        mouseY >= top && mouseY <= bottom);

    // On Point Enter
    if (inside && !isMouseInside)
    {
        isMouseInside = true;
        OnPointEnter();
    }
    // On Point Exit
    else if (!inside && isMouseInside)
    {
        isMouseInside = false;
        OnPointExit();
    }
    // On Click
    if (inside && Input::GetMouseButtonDown(0))
    {
        OnClick();
    }
}

inline void Button::OnClick()
{
    OutputDebugStringA("[Woo Engine] Button Event! OnClick()\n");
    onClickListeners.Invoke();
}

inline void Button::OnPointEnter()
{
    OutputDebugStringA("[Woo Engine] Button Event! OnPointEnter()\n");
    
    // TODO :: 채도 변환 함수로 바꾸기
    // 렌더모드를 바꿔야해서 안바꾸는게 나을 것 같음
    ImageRenderer* ir = this->gameObject->GetComponent<ImageRenderer>();
    if (ir) ir->SetAlpha(0.8);

    onPointEnterListeners.Invoke();
}

inline void Button::OnPointExit()
{
    OutputDebugStringA("[Woo Engine] Button Event! OnPointExit()\n");
    
    // TODO :: 채도 변환 함수로 바꾸기
    // 렌더모드를 바꿔야해서 안바꾸는게 나을 것 같음
    ImageRenderer* ir = this->gameObject->GetComponent<ImageRenderer>();
    if (ir) ir->SetAlpha(1);

    onPointExitListeners.Invoke();
}