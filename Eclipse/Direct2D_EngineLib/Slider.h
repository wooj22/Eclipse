#pragma once
#include "I_UI.h"
#include "MultiDelegate.h"
#include "UISystem.h"

/* [Slider Conponent]
* 하위 오브젝트의 Gauge와 Handle을 참조하여 슬라이더 value를 계산하고
* onValueChanged이벤트를 발생시켜주는 컴포넌트이다.
*/

class RectTransform;
class Slider : public I_UI
{
private:
    // 아직 뭘 참조해서 만져줘야하는지 모르겠음

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

