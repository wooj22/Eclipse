#pragma once
#include <vector>
#include <random>
#include <string>
#include "../Direct2D_EngineLib/Vector2.h"

class Aron_Scene;
class GameObject;

enum class HonType
{
    A,
    B,
    C,
    D
};

enum class WaveType
{
    WAVE_1,
    WAVE_2,
    WAVE_3,
    BOSS_WAVE
};

enum class WaveState
{
    INACTIVE,
    ACTIVE,
    COMPLETED,
    FAILED
};

struct WaveConfig
{
    WaveType type;
    float duration;
    std::vector<HonType> allowedTypes;
    float spawnInterval;
    int maxSimultaneous;
    float minSpawnInterval;
    std::wstring description;
};

class WaveSystem
{
private:
    Aron_Scene* m_scene;
    std::vector<WaveConfig> m_waveConfigs;
    int m_currentWaveIndex;
    WaveState m_currentState;
    float m_waveStartTime;
    float m_lastSpawnTime;
    float m_accumulatedTime;
    std::vector<GameObject*> m_activeHons;
    int m_totalSpawned;
    int m_totalDefeated;
    float m_currentSpawnInterval;
    bool m_bossSpawned;
    std::random_device m_rd;
    std::mt19937 m_gen;
    float m_spawnY;
    float m_spawnRangeX;
    
public:
    WaveSystem(Aron_Scene* scene);
    ~WaveSystem();
    
    void InitializeWaves();
    void StartWave(int waveIndex);
    void UpdateWave(float deltaTime);
    void CompleteCurrentWave();
    void FailCurrentWave();
    void ResetSystem();
    
    void SpawnRandomHon();
    void SpawnBoss();
    GameObject* CreateHonByType(HonType type);
    HonType GetRandomAllowedType();
    Vector2 GetRandomSpawnPosition();
    
    bool IsWaveActive() const { return m_currentState == WaveState::ACTIVE; }
    bool IsWaveCompleted() const { return m_currentState == WaveState::COMPLETED; }
    WaveType GetCurrentWaveType() const;
    float GetElapsedTime() const;
    float GetRemainingTime() const;
    int GetCurrentWaveIndex() const { return m_currentWaveIndex; }
    
    void RegisterHon(GameObject* hon);
    void UnregisterHon(GameObject* hon);
    void OnHonDefeated();
    void CleanupInactiveHons();
    int GetActiveHonCount() const;
    int GetTotalDefeated() const { return m_totalDefeated; }
    
    void AdjustDifficulty();
    
    std::wstring GetWaveStatusText() const;
    std::wstring GetTimerText() const;
    std::wstring GetProgressText() const;
};