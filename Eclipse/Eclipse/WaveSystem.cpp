#include "WaveSystem.h"
#include "Aron_Scene.h"
#include "../Direct2D_EngineLib/Time.h"
#include <algorithm>

WaveSystem::WaveSystem(Aron_Scene* scene)
    : m_scene(scene)
    , m_currentWaveIndex(-1)
    , m_currentState(WaveState::INACTIVE)
    , m_waveStartTime(0.0f)
    , m_lastSpawnTime(0.0f)
    , m_accumulatedTime(0.0f)
    , m_totalSpawned(0)
    , m_totalDefeated(0)
    , m_currentSpawnInterval(2.0f)
    , m_bossSpawned(false)
    , m_gen(m_rd())
    , m_spawnY(800.0f)
    , m_spawnRangeX(800.0f)
{
    InitializeWaves();
}

WaveSystem::~WaveSystem()
{
    ResetSystem();
}

void WaveSystem::InitializeWaves()
{
    m_waveConfigs.clear();
    
    WaveConfig config;
    
    // 웨이브 1
    config.type = WaveType::WAVE_1;
    config.duration = 70.0f;
    config.allowedTypes = {HonType::A, HonType::B};
    config.spawnInterval = 2.5f;
    config.maxSimultaneous = 8;
    config.minSpawnInterval = 1.0f;
    config.description = L"Tutorial - Chain Reaction";
    m_waveConfigs.push_back(config);
    
    // 웨이브 2
    config.type = WaveType::WAVE_2;
    config.duration = 70.0f;
    config.allowedTypes = {HonType::A, HonType::B, HonType::C};
    config.spawnInterval = 2.0f;
    config.maxSimultaneous = 10;
    config.minSpawnInterval = 0.8f;
    config.description = L"Chain Usage - Enhanced with C";
    m_waveConfigs.push_back(config);
    
    // 웨이브 3
    config.type = WaveType::WAVE_3;
    config.duration = 70.0f;
    config.allowedTypes = {HonType::A, HonType::B, HonType::C, HonType::D};
    config.spawnInterval = 1.5f;
    config.maxSimultaneous = 12;
    config.minSpawnInterval = 0.5f;
    config.description = L"High Difficulty - Soul Management";
    m_waveConfigs.push_back(config);
    
    // 보스 웨이브
    config.type = WaveType::BOSS_WAVE;
    config.duration = 80.0f;
    config.allowedTypes = {HonType::A, HonType::B, HonType::C, HonType::D};
    config.spawnInterval = 1.0f;
    config.maxSimultaneous = 15;
    config.minSpawnInterval = 0.3f;
    config.description = L"Boss Battle - Soul Collision Combat";
    m_waveConfigs.push_back(config);
}

void WaveSystem::StartWave(int waveIndex)
{
    if (waveIndex < 0 || waveIndex >= static_cast<int>(m_waveConfigs.size()))
        return;
        
    ResetSystem();
    
    m_currentWaveIndex = waveIndex;
    m_currentState = WaveState::ACTIVE;
    m_accumulatedTime = 0.0f;
    m_waveStartTime = 0.0f;
    m_lastSpawnTime = 0.0f;
    m_totalSpawned = 0;
    m_totalDefeated = 0;
    m_currentSpawnInterval = m_waveConfigs[waveIndex].spawnInterval;
    
    if (m_waveConfigs[waveIndex].type == WaveType::BOSS_WAVE)
    {
        m_bossSpawned = false;
    }
    
    char debugMsg[200];
    sprintf_s(debugMsg, "Wave %d started! Duration: %.1fs\n", 
        waveIndex + 1, m_waveConfigs[waveIndex].duration);
    OutputDebugStringA(debugMsg);
}

void WaveSystem::UpdateWave(float deltaTime)
{
    if (m_currentState != WaveState::ACTIVE || m_currentWaveIndex < 0)
        return;
        
    m_accumulatedTime += deltaTime;
    
    const WaveConfig& config = m_waveConfigs[m_currentWaveIndex];
    
    if (m_accumulatedTime >= config.duration)
    {
        CompleteCurrentWave();
        return;
    }
    
    CleanupInactiveHons();
    
    if (m_accumulatedTime - m_lastSpawnTime >= m_currentSpawnInterval)
    {
        if (GetActiveHonCount() < config.maxSimultaneous)
        {
            SpawnRandomHon();
            m_lastSpawnTime = m_accumulatedTime;
        }
    }
    
    if (config.type == WaveType::BOSS_WAVE && !m_bossSpawned && m_accumulatedTime >= 30.0f)
    {
        SpawnBoss();
        m_bossSpawned = true;
    }
    
    AdjustDifficulty();
}

void WaveSystem::SpawnRandomHon()
{
    if (m_currentWaveIndex < 0) return;
    
    HonType selectedType = GetRandomAllowedType();
    
    // 시뮬레이션: Hon 스폰 카운트만 증가
    m_totalSpawned++;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Wave %d: Simulated Hon%c spawn (%d total)\n", 
        m_currentWaveIndex + 1, 'A' + static_cast<int>(selectedType), m_totalSpawned);
    OutputDebugStringA(debugMsg);
}

GameObject* WaveSystem::CreateHonByType(HonType type)
{
    // Hon 객체 생성 비활성화 - 기존 우씬 객체 사용
    OutputDebugStringA("Wave system active - using existing objects only\n");
    return nullptr;
}

void WaveSystem::SpawnBoss()
{
    // 보스 스폰 비활성화 - 기존 객체 사용
    OutputDebugStringA("Boss wave active - using existing boss objects\n");
    m_bossSpawned = true;
}

HonType WaveSystem::GetRandomAllowedType()
{
    if (m_currentWaveIndex < 0) return HonType::A;
    
    const WaveConfig& config = m_waveConfigs[m_currentWaveIndex];
    if (config.allowedTypes.empty()) return HonType::A;
    
    std::uniform_int_distribution<int> dis(0, static_cast<int>(config.allowedTypes.size()) - 1);
    return config.allowedTypes[dis(m_gen)];
}

Vector2 WaveSystem::GetRandomSpawnPosition()
{
    std::uniform_real_distribution<float> xDis(-m_spawnRangeX / 2, m_spawnRangeX / 2);
    return Vector2(xDis(m_gen), m_spawnY);
}

void WaveSystem::RegisterHon(GameObject* hon)
{
    if (hon)
    {
        m_activeHons.push_back(hon);
    }
}

void WaveSystem::UnregisterHon(GameObject* hon)
{
    auto it = std::find(m_activeHons.begin(), m_activeHons.end(), hon);
    if (it != m_activeHons.end())
    {
        m_activeHons.erase(it);
    }
}

void WaveSystem::OnHonDefeated()
{
    m_totalDefeated++;
}

void WaveSystem::CleanupInactiveHons()
{
    m_activeHons.erase(
        std::remove_if(m_activeHons.begin(), m_activeHons.end(),
            [](GameObject* hon) { return !hon || !hon->IsActive(); }),
        m_activeHons.end()
    );
}

void WaveSystem::AdjustDifficulty()
{
    if (m_currentWaveIndex < 0) return;
    
    const WaveConfig& config = m_waveConfigs[m_currentWaveIndex];
    
    float timeRatio = m_accumulatedTime / config.duration;
    m_currentSpawnInterval = config.spawnInterval * (1.0f - timeRatio * 0.3f);
    
    if (m_currentSpawnInterval < config.minSpawnInterval)
    {
        m_currentSpawnInterval = config.minSpawnInterval;
    }
}

void WaveSystem::CompleteCurrentWave()
{
    m_currentState = WaveState::COMPLETED;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Wave %d completed!\n", m_currentWaveIndex + 1);
    OutputDebugStringA(debugMsg);
}

void WaveSystem::FailCurrentWave()
{
    m_currentState = WaveState::FAILED;
    OutputDebugStringA("Wave failed!\n");
}

void WaveSystem::ResetSystem()
{
    m_currentState = WaveState::INACTIVE;
    m_currentWaveIndex = -1;
    m_accumulatedTime = 0.0f;
    m_waveStartTime = 0.0f;
    m_lastSpawnTime = 0.0f;
    m_totalSpawned = 0;
    m_totalDefeated = 0;
    
    for (auto* hon : m_activeHons)
    {
        if (hon && hon->IsActive())
        {
            hon->SetActive(false);
        }
    }
    m_activeHons.clear();
    
    m_bossSpawned = false;
    
    OutputDebugStringA("Wave system reset!\n");
}

WaveType WaveSystem::GetCurrentWaveType() const
{
    if (m_currentWaveIndex >= 0 && m_currentWaveIndex < static_cast<int>(m_waveConfigs.size()))
    {
        return m_waveConfigs[m_currentWaveIndex].type;
    }
    return WaveType::WAVE_1;
}

float WaveSystem::GetElapsedTime() const
{
    return m_accumulatedTime;
}

float WaveSystem::GetRemainingTime() const
{
    if (m_currentWaveIndex >= 0 && m_currentWaveIndex < static_cast<int>(m_waveConfigs.size()))
    {
        return m_waveConfigs[m_currentWaveIndex].duration - m_accumulatedTime;
    }
    return 0.0f;
}

std::wstring WaveSystem::GetWaveStatusText() const
{
    if (m_currentWaveIndex < 0)
    {
        return L"Wave Ready - Press H/J/K/L to Start";
    }
    
    const WaveConfig& config = m_waveConfigs[m_currentWaveIndex];
    wchar_t statusText[200];
    swprintf_s(statusText, L"Wave %d: %s", 
        m_currentWaveIndex + 1, config.description.c_str());
    
    return std::wstring(statusText);
}

std::wstring WaveSystem::GetTimerText() const
{
    if (m_currentWaveIndex < 0)
    {
        return L"--s";
    }
    
    float remaining = GetRemainingTime();
    int seconds = static_cast<int>(remaining + 1.0f);  // +1로 70초부터 시작
    
    wchar_t timerText[20];
    swprintf_s(timerText, L"%ds", seconds);
    
    return std::wstring(timerText);
}

std::wstring WaveSystem::GetProgressText() const
{
    wchar_t progressText[100];
    swprintf_s(progressText, L"Active: %d | Spawned: %d | Defeated: %d", 
        GetActiveHonCount(), m_totalSpawned, m_totalDefeated);
    
    return std::wstring(progressText);
}

int WaveSystem::GetActiveHonCount() const 
{ 
    return static_cast<int>(m_activeHons.size()); 
}