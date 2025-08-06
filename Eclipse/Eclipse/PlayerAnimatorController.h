#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

#include "PlayerAnimationData.h"


/*------------ Animation Controller ------------*/
class PlayerAnimatorController : public AnimatorController
{
    bool isSkillAvailable = false;

public:
    void SetSkillAvailable(bool available) { isSkillAvailable = available; }

    //std::string GetCurrentPrefix() const {
    //    return isSkillAvailable ? "Y_" : "N_";
    //}

public:
    // Clips
    PlayerIdleClip* idleClip = nullptr;
    PlayerWalkClip* walkClip = nullptr;
    PlayerJumpClip* jumpClip = nullptr;
    PlayerHangingClip* hangingClip = nullptr;
    PlayerAttackClip* attackClip = nullptr;
    // PlayerFallClip* fallClip = nullptr;
    PlayerDashClip* dashClip = nullptr;

    PlayerIdleClip_Y* idleClip_Y = nullptr;
    PlayerWalkClip_Y* walkClip_Y = nullptr;
    PlayerJumpClip_Y* jumpClip_Y = nullptr;
    PlayerHangingClip_Y* hangingClip_Y = nullptr;
    PlayerAttackClip_Y* attackClip_Y = nullptr;
    PlayerDashClip_Y* dashClip_Y = nullptr;

    // States
    PlayerIdleState* idleState = nullptr;
    PlayerWalkState* walkState = nullptr;
    PlayerJumpState* jumpState = nullptr;
    PlayerHangingState* hangingState = nullptr;
    PlayerAttackState* attackState = nullptr;
    // PlayerFallState* fallState = nullptr;
    PlayerDashState* dashState = nullptr;

    PlayerIdleState_Y* idleState_Y = nullptr;
    PlayerWalkState_Y* walkState_Y = nullptr;
    PlayerJumpState_Y* jumpState_Y = nullptr;
    PlayerHangingState_Y* hangingState_Y = nullptr;
    PlayerAttackState_Y* attackState_Y = nullptr;
    // PlayerFallState_Y* fallState_Y = nullptr;
    PlayerDashState_Y* dashState_Y = nullptr;

public: 
    PlayerAnimatorController()
    {
        // [ clip 생성 ]
        idleClip = new PlayerIdleClip();
        walkClip = new PlayerWalkClip();
        jumpClip = new PlayerJumpClip();
        hangingClip = new PlayerHangingClip();
        attackClip = new PlayerAttackClip();
        // fallClip = new PlayerFallClip();
        dashClip = new PlayerDashClip();

        idleClip_Y = new PlayerIdleClip_Y();
        walkClip_Y = new PlayerWalkClip_Y();
        jumpClip_Y = new PlayerJumpClip_Y();
        hangingClip_Y = new PlayerHangingClip_Y();
        attackClip_Y = new PlayerAttackClip_Y();
        dashClip_Y = new PlayerDashClip_Y();



        // [ state 생성 ]

        idleState = new PlayerIdleState(idleClip, this);
        walkState = new PlayerWalkState(walkClip, this);
        jumpState = new PlayerJumpState(jumpClip, this);
        hangingState = new PlayerHangingState(hangingClip, this);
        attackState = new PlayerAttackState(attackClip, this);
        // fallState = new PlayerFallState(fallClip, this);
        dashState = new PlayerDashState(dashClip, this);

        idleState_Y = new PlayerIdleState_Y(idleClip_Y, this);
        walkState_Y = new PlayerWalkState_Y(walkClip_Y, this);
        jumpState_Y = new PlayerJumpState_Y(jumpClip_Y, this);
        hangingState_Y = new PlayerHangingState_Y(hangingClip_Y, this);
        attackState_Y = new PlayerAttackState_Y(attackClip_Y, this);
        dashState_Y = new PlayerDashState_Y(dashClip_Y, this);



        // [ state 등록 ]

        AddState(idleState);
        AddState(walkState);
        AddState(jumpState);
        AddState(hangingState);
        AddState(attackState);
        // AddState(fallState);
        AddState(dashState);

        AddState(idleState_Y);
        AddState(walkState_Y);
        AddState(jumpState_Y);
        AddState(hangingState_Y);
        AddState(attackState_Y);
        AddState(dashState_Y);


        // 초기 상태
        ChangeAnimation(idleState);
    }

    ~PlayerAnimatorController() override
    {
        delete idleClip;
        delete walkClip;
        delete jumpState;
        delete hangingState;
        delete attackState;
        // delete fallState;
        delete dashState;

        delete idleClip_Y;
        delete walkClip_Y;
        delete jumpState_Y;
        delete hangingState_Y;
        delete attackState_Y;
        delete dashState_Y;
    }

    //void SetSkillSensitiveBool(const std::string& baseName, bool value)
    //{
    //    std::string fullName = (isSkillAvailable ? "Y_" : "N_") + baseName;
    //    SetBool(fullName, value);
    //}

    void PlaySkillSensitiveAnimation(const std::string& baseName)
    {
        std::string prefix = isSkillAvailable ? "Y_" : "N_";
        std::string fullName = prefix + baseName;

        if (curState && curState->clip && curState->clip->name == fullName)   return;

        float savedTime = 0.0f;

        // baseName이 같을 때만 시간 이어받기
        if (curState && curState->clip)
        {
            std::string curBaseName = curState->clip->name;
            if (curBaseName.length() > 2)
                curBaseName = curBaseName.substr(2); // "Y_" 또는 "N_" 제거

            if (curBaseName == baseName)
                savedTime = currentTime; // 같은 동작이므로 시간 유지
        }

        auto it = map_state.find(fullName);
        if (it != map_state.end())
        {
            if (curState)
                curState->Exit();

            curState = it->second;
            currentTime = savedTime;
            currentFrameIndex = curState->clip->GetFrameIndexAtTime(currentTime);
            playing = true;

            curState->Enter();
        }
        else
        {
            OutputDebugStringA(("애니메이션 상태 없음: " + fullName + "\n").c_str());
        }
    }
};
