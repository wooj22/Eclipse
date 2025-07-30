#pragma once
#include "Component.h"
#include "MultiDelegate.h"
#include "ButtonSystem.h"

/* [Button Conponent]
* <UI>�� ���� ������ OnClick() �̺�Ʈ�� �����ϰ� �߻���Ű�ϴ� ������Ʈ
* �̺�Ʈ �߻��� onClickListeners�� ��ϵ� �ݹ� �Լ��� ��� ȣ���Ų��.
* RectTransform�� ������ GameObject�� ��ϵǾ�� �������� Button�� ����� Ȱ���� �� �ִ�.
*/

class RectTransform;
class Button : public Component
{
private:
    RectTransform* rectTransform;     // button ���� get

public:
    MultiDelegate<> onClickListeners; // callback functions

public:
    // component cycle
    Button() = default;
    ~Button() override = default;

    void OnEnable_Inner() override final;
	void OnDisable_Inner() override final;
    void Update();
    void OnDestroy_Inner() override final;

private:
    void OnClick(); // event
};

