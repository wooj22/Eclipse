#include "Animator.h"
#include "AnimatorController.h"
#include "SpriteRenderer.h"
#include "ImageRenderer.h"
#include "GameObject.h"
#include "Time.h"

void Animator::OnEnable_Inner()
{
    AnimatorSystem::Get().Regist(this);
    spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
    imageRenderer = gameObject->GetComponent<ImageRenderer>();
}

void Animator::OnDisable_Inner()
{
	AnimatorSystem::Get().Unregist(this);
}

void Animator::Update()
{
    if (!controller) return;

    // animation update (controller->state->clip)
    controller->Update(Time::GetDeltaTime());

    // sprite update
    if(spriteRenderer) spriteRenderer->sprite = controller->GetCurrentSprite();
    else if(imageRenderer) imageRenderer->sprite = controller->GetCurrentSprite();
}

void Animator::OnDestroy_Inner()
{
    AnimatorSystem::Get().Unregist(this);
}

/// set animation controller
void Animator::SetController(AnimatorController* newController)
{
    if (controller) delete controller;
    controller = newController;
}