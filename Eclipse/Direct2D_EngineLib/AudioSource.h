#pragma once
#include "Component.h"
#include "AudioSystem.h"

/* [AudioSource Component]
* 
*/

class AudioSource : public Component
{
public:
    // component cycle
    AudioSource() = default;
    ~AudioSource() override {}

    void OnEnable_Inner() override final;
    void OnDisable_Inner() override final;
    void Update();
    void OnDestroy_Inner() override final;
};

