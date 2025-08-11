#include "InvokeSystem.h"

// static ¸â¹ö Á¤ÀÇ
std::vector<InvokeTask> InvokeSystem::tasks;

void InvokeSystem::Invoke(float delay, std::function<void()> func)
{
    tasks.push_back({ delay, func });
}

template<typename T>
void InvokeSystem::Invoke(float delay, T* instance, void (T::* method)())
{
    tasks.push_back({
        delay,
        [instance, method]() {
            (instance->*method)();
        }
        });
}

void InvokeSystem::Update()
{
    for (auto it = tasks.begin(); it != tasks.end(); )
    {
        it->timeRemaining -= Time::GetDeltaTime();
        if (it->timeRemaining <= 0.0f)
        {
            it->func();
            it = tasks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
