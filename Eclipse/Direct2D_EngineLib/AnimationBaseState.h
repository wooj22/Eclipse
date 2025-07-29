#pragma once
#include "AnimationClip.h"

/* [AnimationBaseState]
* AnimatorController의 FSM에 등록될 Base State
* 게임 콘텐츠에서 Animation Clip별 State를 생성하고, FSM 상태 전환 로직을 작성하면 된다.
* Frame은 AnimatorController에서 update하고 있으므로 state 내부에서는
* 전이 조건을 넣어 다른 animation state로 전환하는 로직을 작성하고,    // 물론 외부에서 PlayAnimation해도 된다.
* Sound, Effect등의 타이밍을 맞추어 재생하는 코드를 작성할 수 있다.
*/

class AnimatorController;
class AnimationBaseState
{
public:
    AnimationClip* clip = nullptr;                  // clip state
    AnimatorController* controller = nullptr;       // state가 등록될 controller 참조 - 전이조건값 get
    
    AnimationBaseState(AnimationClip* c, AnimatorController* ac) : 
        clip(c), controller(ac) {}
    virtual ~AnimationBaseState() = default;

    // animator controller에서 호출
    virtual void Enter() = 0;
    virtual void Update(float dt) = 0;
    virtual void Exit() = 0;
};

