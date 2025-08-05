#include "AnimatorController.h"

// frame update
void AnimatorController::Update(float deltaTime)
{
    {
        if (!curState || !curState->clip || !playing)
            return;

        // loop cheak
        currentTime += deltaTime;
        if (currentTime >= curState->clip->duration)
        {
            if (curState->clip->loop) currentTime = 0.0f;
            else
            {
                currentTime = curState->clip->duration;
                playing = false;
            }
        }

        // frame update
        int frameIndex = 0;
        for (size_t i = 0; i < curState->clip->frames.size(); ++i)
        {
            if (currentTime >= curState->clip->frames[i].time)
                frameIndex = (int)i;
            else
                break;
        }

        currentFrameIndex = frameIndex;
        curState->Update(deltaTime);
    }
}

// add state
void AnimatorController::AddState(AnimationBaseState* state)
{
    string name = state->clip->name;
    map_state[name] = state;
}

// play animation with clip name
void AnimatorController::PlayAnimation(const string& clipName)
{
    auto it = map_state.find(clipName);
    if (it != map_state.end())
        ChangeAnimation(it->second);
    else
        OutputDebugStringA(("clip name이 없습니다. " + clipName + "\n").c_str());
}

// animation state(Clip) change
void AnimatorController::ChangeAnimation(AnimationBaseState* state)
{
    if (curState)
        curState->Exit();

    curState = state;
    currentTime = 0.0f;
    currentFrameIndex = 0;
    playing = true;

    if (curState)
        curState->Enter();
}

// get sprite
shared_ptr<Sprite> AnimatorController::GetCurrentSprite()
{
    if (!curState || !curState->clip) return nullptr;
    auto sprite = curState->clip->frames[currentFrameIndex].sprite;
    if (!sprite)
        OutputDebugStringA("currentFrameIndex에 해당하는 sprite가 nullptr입니다.\n");
    return curState->clip->frames[currentFrameIndex].sprite;
}

// conditions
void AnimatorController::SetBool(const std::string& name, bool value)
{
    map_boolParams[name] = value;
}

bool AnimatorController::GetBool(const string& name)
{
    auto it = map_boolParams.find(name);
    if (it != map_boolParams.end())
        return it->second;
    return false;
}

void AnimatorController::SetFloat(const string& name, float value)
{
    map_floatParams[name] = value;
}

float AnimatorController::GetFloat(const string& name)
{
    auto it = map_floatParams.find(name);
    if (it != map_floatParams.end())
        return it->second;
    return 0.0f;
}

void AnimatorController::SetInt(const string& name, int value)
{
    map_intParams[name] = value;
}

int AnimatorController::GetInt(const string& name)
{
    auto it = map_intParams.find(name);
    if (it != map_intParams.end())
        return it->second;
    return 0;
}