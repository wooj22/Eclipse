#pragma once
#include "I_UI.h"
#include "MultiDelegate.h"
#include "UISystem.h"

/* [Slider Conponent]
* 하위 오브젝트의 Gauge와 Handle을 참조하여 슬라이더 value를 계산하고
* onValueChanged이벤트를 발생시켜주는 컴포넌트이다.
* 
* handle의 rect는 slideArea의 자식임
* slideArea내의 handle 드래그(x)를 감지하여 handle의 위치를 옮기고 -> handlePos.x/slideAreaSize.x = value
* value에 따라 gauge.fillAmount = value
* value가 바뀐것을 감지하여 onValueChangedListeners이벤트 발생
*/

class RectTransform;
class ImageRenderer;
class Slider : public I_UI
{
private:
    float value = 0.5f;         // slider fill amount

public:
    // Slider GameObject Reacttransfom (Area)
    RectTransform* slideArea = nullptr;
    
    // child component
    RectTransform* handle = nullptr;
    ImageRenderer* gauge = nullptr;

    // component cycle
    Slider() = default;
    ~Slider() override = default;

    void OnEnable_Inner() override final;
    void OnDisable_Inner() override final;
    void Update();
    void OnDestroy_Inner() override final;

public:
    // funcs
    float GetValue() { return value; }
public:
    /* Event */
    MultiDelegate<> onValueChangedListeners;       // On Value Changed
    void OnValueChanged();
};

