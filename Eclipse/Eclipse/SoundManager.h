#pragma once
#include "../Direct2D_EngineLib/Singleton.h"
#include "../Direct2D_EngineLib/AudioSystem.h"
#include <algorithm>

template<typename T>
T Clamp(T value, T minVal, T maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

class SoundManager : public Singleton<SoundManager>
{
private:
    float masterVolume = 1.0f;
    float bgmVolume = 1.0f;
    float sfxVolume = 1.0f;
    float ambVolume = 1.0f;

    float bgmScale = 1.0f;  
    float bgmBaseScale = 0.5f;   // 원래 BGM 커서 기본으로 줄이는 값


private:

    void ApplyVolumes()
    {
        AudioSystem::Get().SetMasterVolume(masterVolume);

        // bgmVolume는 0~1 범위 유지, 실제 출력 볼륨 계산 시만 곱함
        float finalBGM = Clamp(bgmVolume, 0.0f, 1.0f) * bgmScale * bgmBaseScale;
        AudioSystem::Get().SetBGMVolume(finalBGM);
        AudioSystem::Get().SetSFXVolume(sfxVolume);
        AudioSystem::Get().SetAMBVolume(ambVolume);
    }

    struct FadeInfo
    {
        float current = 1.0f;
        float target = 1.0f;
        float duration = 0.0f;
        float elapsed = 0.0f;
        bool active = false;
    };

    FadeInfo masterFade;

public:

    SoundManager() = default;
    ~SoundManager() = default;

    void Update(float deltaTime)
    {
        if (!masterFade.active) return;

        masterFade.elapsed += deltaTime;
        float t = (((masterFade.elapsed / masterFade.duration) < (1.0f)) ? (masterFade.elapsed / masterFade.duration) : (1.0f));
        masterFade.current = masterFade.current + t * (masterFade.target - masterFade.current);

        if (t >= 1.0f)
        {
            masterVolume = masterFade.target; // 완료 시 옵션값 업데이트
            masterFade.active = false;
        }
    }


    // 볼륨 세팅 (옵션창에서 사용하는 값)
    void SetMasterVolume(float volume) { masterVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }
    void SetBGMVolume(float volume) { bgmVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }
    void SetSFXVolume(float volume) { sfxVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }
    void SetAMBVolume(float volume) { ambVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }

    // 현재 옵션 볼륨 
    float GetMasterVolume() const { return masterVolume; }
    float GetBGMVolume() const { return bgmVolume; }
    float GetSFXVolume() const { return sfxVolume; }
    float GetAMBVolume() const { return ambVolume; }


    // BGM 스케일
    void SetBGMScale(float scale) { bgmScale = Clamp(scale, 0.0f, 1.0f); ApplyVolumes(); }
    float GetBGMScale() const { return bgmScale; }

    float GetFinalBGMVolume() const
    {
        return Clamp(bgmVolume, 0.0f, 1.0f) * bgmScale * bgmBaseScale;
    }


    // [ 페이드 인/아웃 ]

    void FadeMaster(float targetVolume, float duration)
    {
        masterFade.target = Clamp(targetVolume, 0.0f, 1.0f);
        masterFade.current = masterVolume;
        masterFade.duration = duration;
        masterFade.elapsed = 0.0f;
        masterFade.active = true;
    }

    // 페이드 인: 마스터 볼륨을 1로 점점 올림
    void FadeInMaster(float duration) { FadeMaster(1.0f, duration); }

    // 페이드 아웃: 마스터 볼륨을 0으로 점점 줄임
    void FadeOutMaster(float duration) { FadeMaster(0.0f, duration); }
};