#pragma once
// #include "../Direct2D_EngineLib/Singleton.h"
#include "../Direct2D_EngineLib/AudioSystem.h"
#include "../Direct2D_EngineLib/Script.h"

#include <algorithm>

template<typename T>
T Clamp(T value, T minVal, T maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

class SoundManager : public Script
{
private:
    float masterVolume = 1.0f;
    float bgmVolume = 1.0f;
    float sfxVolume = 1.0f;
    float ambVolume = 1.0f;

    float bgmScale = 1.0f;  
    float bgmBaseScale = 0.3f;   // ���� BGM Ŀ�� �⺻���� ���̴� ��


private:

    void ApplyVolumes()
    {
        // ���� ���� = �ɼ� ���� �� ������ �� �⺻ BGM ũ�� ����
        AudioSystem::Get().SetMasterVolume(masterVolume);
        AudioSystem::Get().SetBGMVolume(bgmVolume * bgmScale * bgmBaseScale);
        AudioSystem::Get().SetSFXVolume(sfxVolume);
        AudioSystem::Get().SetAMBVolume(ambVolume);
    }


public:

    SoundManager() = default;
    ~SoundManager() = default;

    // ���� ���� (�ɼ�â���� ����ϴ� ��)
    void SetMasterVolume(float volume) { masterVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }
    void SetBGMVolume(float volume) { bgmVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }
    void SetSFXVolume(float volume) { sfxVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }
    void SetAMBVolume(float volume) { ambVolume = Clamp(volume, 0.0f, 1.0f); ApplyVolumes(); }

    // ���� �ɼ� ���� ��������
    float GetMasterVolume() const { return masterVolume; }
    float GetBGMVolume() const { return bgmVolume; }
    float GetSFXVolume() const { return sfxVolume; }
    float GetAMBVolume() const { return ambVolume; }


    // BGM ������
    void SetBGMScale(float scale) { bgmScale = Clamp(scale, 0.0f, 1.0f); ApplyVolumes(); }
    float GetBGMScale() const { return bgmScale; }
};