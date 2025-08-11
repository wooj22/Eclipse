#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "Attack_State.h"
#include "Dash_State.h"
#include "BulletTime_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"


void Walk_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Walk_State] Player의 Walk_State 진입 \n");

    // 초기화 
    fsm->GetPlayerFSM()->holdTime = 0.0f;
    fsm->GetPlayerFSM()->isHolding = false;
    fsm->GetPlayerFSM()->timer = 0.0f;

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Walk", true);

    // 초기 재생 
    PlayRandomFootstep(fsm);
}

void Walk_State::Update(MovementFSM* fsm)
{
    // [ Jump ]
    if (fsm->GetPlayerFSM()->GetIsSpace() && fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->OnJump(JumpPhase::NormalJump);
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
    }

    // [ Idle ]
    if (!(fsm->GetPlayerFSM()->GetIsA() || fsm->GetPlayerFSM()->GetIsD()))
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
    }

    // [ Attack / Bullet ]
    if (fsm->GetPlayerFSM()->GetIsLButton())
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold) fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());

    }
    else
    {
        // [ Attack ]
        if (fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold) fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());

        // 초기화
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }

    // [ Dash ]
    if (fsm->GetPlayerFSM()->GetisShift() && GameManager::Get().CheckUnlock(SkillType::Dash) && fsm->GetPlayerFSM()->CanDash())
    {
        fsm->ChangeState(std::make_unique<Dash_State>());
        return;
    }

    // --- 발소리 랜덤 재생 로직 ---
    auto audioSource = fsm->GetPlayerFSM()->GetAudioSource();
    if (!audioSource->IsPlaying())
    {
        PlayRandomFootstep(fsm);
    }
}

void Walk_State::FixedUpdate(MovementFSM* fsm)
{
    // Move 실행 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = fsm->GetPlayerFSM()->GetInputX() * fsm->GetPlayerFSM()->GetCurSpeed();
}

void Walk_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Walk", false);

    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}



// [ 발소리 랜덤 선택 ]
void Walk_State::PlayRandomFootstep(MovementFSM* fsm)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    int choice = rand() % 4;
    std::shared_ptr<AudioClip> clip;

    switch (choice)
    {
    case 0: clip = fsm->GetPlayerFSM()->SFX_Player_Move1; break;
    case 1: clip = fsm->GetPlayerFSM()->SFX_Player_Move2; break;
    case 2: clip = fsm->GetPlayerFSM()->SFX_Player_Move3; break;
    case 3: clip = fsm->GetPlayerFSM()->SFX_Player_Move4; break;
    }

    auto audioSource = fsm->GetPlayerFSM()->GetAudioSource();
    audioSource->SetClip(clip);
    audioSource->SetLoop(false); // 반복 아님
    audioSource->PlayOneShot();
}