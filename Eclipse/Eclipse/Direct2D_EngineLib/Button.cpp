#include "Button.h"
#include "RectTransform.h"
#include "ImageRenderer.h"
#include "GameObject.h"
#include "Input.h"
#include <iostream>
#include <string>

void Button::OnEnable_Inner()
{
    ButtonSystem::Get().Regist(this);
    rectTransform = this->gameObject->rectTransform;
}

void Button::OnDisable_Inner()
{
	ButtonSystem::Get().Unregist(this);
	rectTransform = nullptr;
}

void Button::OnDestroy_Inner()
{
    ButtonSystem::Get().Unregist(this);
}

void Button::Update() 
{
    if (!rectTransform) return;

    // 1. ���콺 Ŭ�� ����
    if (!Input::GetKeyDown(VK_LBUTTON)) return;

    // 2. ���콺 ��ġ �������� (Ŭ���̾�Ʈ ��ǥ��)
    POINT mouse = Input::GetMouseScreenPosition();
    float mouseX = static_cast<float>(mouse.x);
    float mouseY = static_cast<float>(mouse.y);

    // 3. ��ư ���� ��� (pivot ������ screen ����)
    Vector2 pos = rectTransform->GetPosition();
    D2D1_SIZE_F size = rectTransform->GetSize();
    D2D1_POINT_2F pivot = rectTransform->GetPivot();

    float left = pos.x - (size.width * pivot.x);
    float top = pos.y - (size.height * pivot.y);
    float right = left + size.width;
    float bottom = top + size.height;

    // 4. ���콺�� ��ư ���� �ȿ� �ִ��� Ȯ��
    if (mouseX >= left && mouseX <= right &&
        mouseY >= top && mouseY <= bottom)
    {
        OnClick();  // Ŭ�� �̺�Ʈ ȣ��
    }
}

inline void Button::OnClick()
{
    onClickListeners.Invoke();
}