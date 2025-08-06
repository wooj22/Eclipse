#pragma once
#include <iostream>
#include "AnimationClip.h"
#include "AnimationBaseState.h"
#include "Animator.h"

/* [AnimatorController Asset]
* Animation State Machine (FSM)
* State를 관리하며, 현재 AnimationClip의 프레임을 계산한다.
* AnimationState들을 가지고 State를 전환하여, State의 Enter(), Update(), Exit()를 호출한다.
* Transition(전이)에 활용될 값들을 저장하고, State에서 전이 조건을 활용하여 State를 전환할 수 있도록 한다.
* 게임 콘텐츠에서 이 class를 상속받은 AnimationController에 State를 등록하여 활용하면 된다..
* ex) class PlayerAnimationController : public AnimatorController{}
*     -> 이 안에서 animation clip과 state를 생성하면 된다.
*        그럼 GameObject쪽에서는 미리 정의한 컨트롤러만 생성시키면 깔끔하게 정리됨
*/

class AnimatorController
{
public:
    // animation
    AnimationBaseState* curState = nullptr;     // animation clip state
    float currentTime = 0.0f;                   // current time (clip start time cheak)
    int currentFrameIndex = 0;                  // current frame
    bool playing = false;                       // loop/stop

public:
    // conditions
    unordered_map<string, bool> map_boolParams;
    unordered_map<string, float> map_floatParams;
    unordered_map<string, int> map_intParams;

public:
    // clip state map <name, state>
    unordered_map<string, AnimationBaseState*> map_state;    

public:
    AnimatorController() = default;
    virtual ~AnimatorController() = default;

    void Update(float deltaTime);               // frame update
    void AddState(AnimationBaseState* state);   // add state(state)
    void PlayAnimation(const string& clipName); // play animation("clip name")
    void ChangeAnimation(AnimationBaseState* state); // change animation(state)
    shared_ptr<Sprite> GetCurrentSprite(); // get sprite

public:
    // conditions
    void SetBool(const string& name, bool value);
    bool GetBool(const string& name);
    void SetFloat(const string& name, float value);
    float GetFloat(const string& name);
    void SetInt(const string& name, int value);
    int GetInt(const string& name);
};