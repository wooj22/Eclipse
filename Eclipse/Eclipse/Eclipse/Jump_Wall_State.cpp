#include "Jump_Wall_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"

void Jump_Wall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_Wall_State] 벽 점프 상태 진입\n");

    Rigidbody* rb = fsm->GetPlayerFSM()->GetRigidbody();
    Vector2 force;

    if (fsm->GetPlayerFSM()->GetIsWallLeft())
    {
        force = Vector2(+jumpXPower, wallJumpForce);  // 오른쪽 방향으로 튕김
    }
    else if (fsm->GetPlayerFSM()->GetIsWallRight())
    {
        force = Vector2(-jumpXPower, wallJumpForce);  // 왼쪽 방향으로 튕김
    }

    rb->velocity = Vector2(0, 0); // 수직 속도 초기화
    rb->AddImpulse(force);
}

void Jump_Wall_State::Update(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Jump_Wall_State::FixedUpdate(MovementFSM* fsm)
{

}

void Jump_Wall_State::Exit(MovementFSM* fsm)
{
}