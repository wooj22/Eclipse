#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class NpcIdleClip : public AnimationClip
{
public:
	NpcIdleClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/NPC/NPC_sheet.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/NPC/NPC_Sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/NPC/NPC_Idle_AniClip.json");
	}
};


/*------------ Animation State ------------*/
class NpcIdleState : public AnimationBaseState
{
public:
	NpcIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};



/*---------- Animation Controller ---------*/
class NpcAnimatorController : public AnimatorController
{
public:
	NpcIdleClip* idleClip = nullptr;
	NpcIdleState* idleState = nullptr;

public:
	NpcAnimatorController()
	{
		idleClip = new NpcIdleClip();
		idleState = new NpcIdleState(idleClip, this);

		AddState(idleState);
		ChangeAnimation(idleState);
	}

	~NpcAnimatorController()
	{
		delete idleClip;
		delete idleState;
	}
};