#include "PlayerAnimationData.h"
#include "PlayerAnimatorController.h"

void PlayerIdleState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Walk"))  pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Idle");
}

void PlayerWalkState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Walk");
}

void PlayerJumpState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk"))  pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else if(controller->GetBool("Jump"))   pac->PlaySkillSensitiveAnimation("Player_Jump");
}

void PlayerHangingState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk")) pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Hanging");
}

void PlayerDashState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk")) pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Hanging"))    pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Dash");
}

void PlayerAttackState::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk")) pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Hanging"))    pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Dash"))  pac->PlaySkillSensitiveAnimation("Player_Dash");
    else   pac->PlaySkillSensitiveAnimation("Player_Attack");
}



// [ Y ]

void PlayerIdleState_Y::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Walk"))  pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Idle");
}

void PlayerWalkState_Y::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Walk");
}

void PlayerJumpState_Y::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk"))  pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Hanging")) pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Jump");
}

void PlayerHangingState_Y::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk")) pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Dash"))    pac->PlaySkillSensitiveAnimation("Player_Dash");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Hanging");
}

void PlayerDashState_Y::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk")) pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Hanging"))    pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Attack"))  pac->PlaySkillSensitiveAnimation("Player_Attack");
    else   pac->PlaySkillSensitiveAnimation("Player_Dash");
}

void PlayerAttackState_Y::Update(float dt)
{
    auto pac = dynamic_cast<PlayerAnimatorController*>(controller);
    if (!pac) return;

    if (controller->GetBool("Idle"))  pac->PlaySkillSensitiveAnimation("Player_Idle");
    else if (controller->GetBool("Walk")) pac->PlaySkillSensitiveAnimation("Player_Walk");
    else if (controller->GetBool("Jump"))  pac->PlaySkillSensitiveAnimation("Player_Jump");
    else if (controller->GetBool("Hanging"))    pac->PlaySkillSensitiveAnimation("Player_Hanging");
    else if (controller->GetBool("Dash"))  pac->PlaySkillSensitiveAnimation("Player_Dash");
    else   pac->PlaySkillSensitiveAnimation("Player_Attack");
}
