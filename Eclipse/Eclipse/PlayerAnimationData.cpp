#include "PlayerAnimationData.h"
#include "PlayerAnimatorController.h"

void PlayerIdleState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Walk"))  pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    controller->PlayAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) controller->PlayAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  controller->PlayAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Idle");

    // [ tansition ]
    //if (controller->GetBool("N_Player_Walk") == true)         controller->PlayAnimation("N_Player_Walk");
    //else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    //else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    //else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    //else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");

    //else if (controller->GetBool("Y_Player_Idle") == true)    controller->PlayAnimation("Y_Player_Idle");
    //else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    //else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    //else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    //else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    //else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
}

void PlayerWalkState::Update(float dt)
{
    //auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    //if (!pac) return;

    //if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    //else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    //else if (controller->GetBool("Dash"))    controller->PlayAnimation("Player_Dash");
    //else if (controller->GetBool("Hanging")) controller->PlayAnimation("Player_Hanging");
    //else if (controller->GetBool("Attack"))  controller->PlayAnimation("Player_Attack");
    //else   pac->PlaySkillSensitiveAnimation("Player_Walk");

    // [ tansition ]
    if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");

    else if (controller->GetBool("Y_Player_Idle") == true)    controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
}

void PlayerJumpState::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");

    else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
}

void PlayerHangingState::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");

    else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
}

void PlayerDashState::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");

    else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
}

void PlayerAttackState::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");

    else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
}



// [ Y ]
void PlayerIdleState_Y::Update(float dt)
{
    /*       auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
           if (!pac) return;

           if (controller->GetBool("Walk"))  pac->PlaySkillSensitiveAnimation("Player_Walk");
           else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
           else if (controller->GetBool("Dash"))    controller->PlayAnimation("Player_Dash");
           else if (controller->GetBool("Hanging")) controller->PlayAnimation("Player_Hanging");
           else if (controller->GetBool("Attack"))  controller->PlayAnimation("Player_Attack");
           else   pac->PlaySkillSensitiveAnimation("Player_Idle");*/


           // [ tansition ]
    if (controller->GetBool("Y_Player_Walk") == true)         controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

    else if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Walk") == true)         controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
}

void PlayerJumpState_Y::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

    else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");
}

void PlayerHangingState_Y::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

    else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");
}

void PlayerDashState_Y::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

    else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
    else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
}

void PlayerAttackState_Y::Update(float dt)
{
    // [ tansition ]
    if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
    else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
    else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
    else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
    else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");

    else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
    else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
    else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
    else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
    else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
}