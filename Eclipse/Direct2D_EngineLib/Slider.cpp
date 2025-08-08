#include "Slider.h"
#include "RectTransform.h"
#include "ImageRenderer.h"
#include "GameObject.h"
#include "Input.h"
#include <iostream>
#include <string>

void Slider::OnEnable_Inner()
{
    UISystem::Get().Regist(this);
}

void Slider::OnDisable_Inner()
{
    UISystem::Get().Unregist(this);
}

void Slider::OnDestroy_Inner()
{
    UISystem::Get().Unregist(this);
}

void Slider::Update()
{
    
}

void Slider::OnValueChanged()
{
    OutputDebugStringA("[Woo Engine] Slider Event! OnValueChanged()\n");
    onValueChangedListeners.Invoke();
}