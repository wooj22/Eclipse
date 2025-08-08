#pragma once
#include "I_UI.h"
#include "MultiDelegate.h"
#include "UISystem.h"

/* [Slider Conponent]
* ���� ������Ʈ�� Gauge�� Handle�� �����Ͽ� �����̴� value�� ����ϰ�
* onValueChanged�̺�Ʈ�� �߻������ִ� ������Ʈ�̴�.
*/

class RectTransform;
class Slider : public I_UI
{
private:
    // ���� �� �����ؼ� ��������ϴ��� �𸣰���

public:
    // component cycle
    Slider() = default;
    ~Slider() override = default;

    void OnEnable_Inner() override final;
    void OnDisable_Inner() override final;
    void Update();
    void OnDestroy_Inner() override final;

public:
    /* Event */
    MultiDelegate<> onValueChangedListeners;       // On Value Changed

private:
    void OnValueChanged();
};

