#include "AudioSource.h"


void AudioSource::OnEnable_Inner()
{
    AudioSystem::Get().Regist(this);
}

void AudioSource::OnDisable_Inner()
{
    AudioSystem::Get().Unregist(this);
}

void AudioSource::Update()
{
    
}

void AudioSource::OnDestroy_Inner()
{
    AudioSystem::Get().Unregist(this);
}
