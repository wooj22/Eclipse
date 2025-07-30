#include "Time.h"

LARGE_INTEGER Time::frequency = {};
LARGE_INTEGER Time::initCounter = {};
LARGE_INTEGER Time::prevCounter = {};
LARGE_INTEGER Time::currentCounter = {};
float Time::deltaTime = 0.0f;
float Time::fixedDeltaTime = 0.02f;
float Time::timeScale = 1.0f;

void Time::Init()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&initCounter);
    prevCounter = initCounter;
}

void Time::Update()
{
    QueryPerformanceCounter(&currentCounter);
    float unscaledDelta = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
    prevCounter = currentCounter;

    deltaTime = unscaledDelta * timeScale;
}

float Time::GetDeltaTime()
{
    return deltaTime;
}

float Time::GetFixedDeltaTime()
{
    return fixedDeltaTime;
}

float Time::GetTotalTime()
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return static_cast<float>(now.QuadPart - initCounter.QuadPart) / frequency.QuadPart;
}

void Time::SetTimeScale(float scale)
{
    if (scale < 0.0f) scale = 0.0f;
    timeScale = scale;
}

float Time::GetTimeScale()
{
    return timeScale;
}