#include "WaveSystem.h"
#include "HonA.h"
#include "HonB.h"
#include "HonC.h"
#include "HonD.h"
#include "Boss.h"
#include "GameManager.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Singleton.h"
#include <algorithm>
#include <chrono>

WaveSystem::WaveSystem()
{
    m_currentWaveState = WaveState::IDLE;
    m_waveElapsedTime = 0.0f;
    m_waveDuration = 0.0f;
    m_currentSpawnIndex = 0;
    m_lastSpawnTime = 0.0f;
    m_spawnedCount = 0;
    m_destroyedCount = 0;
    m_activeBoss = nullptr;
    m_gameManager = nullptr;
    
    // Initialize random generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    m_randomGen = std::mt19937(static_cast<unsigned int>(seed));
    m_xDistribution = std::uniform_real_distribution<float>(-960.0f + SPAWN_MARGIN, 960.0f - SPAWN_MARGIN);
}

WaveSystem::~WaveSystem()
{
}

void WaveSystem::Awake()
{
    // GameManager is singleton
    m_gameManager = &GameManager::Get();
}

void WaveSystem::Start()
{
}

void WaveSystem::Update()
{
    // Wait for GameManager to signal wave start
    if (!m_gameManager->isWave)
    {
        // Reset flag when not in wave
        if (m_currentWaveState != WaveState::IDLE)
        {
            m_currentWaveState = WaveState::IDLE;
            m_waveStartCheck = true;
        }
        return;
    }

    // Start wave when GameManager signals
    if (m_waveStartCheck && m_gameManager->isWave)
    {
		StartWave(m_gameManager->waveCount);
	    m_waveStartCheck = false;
    }
    
    // Update wave elapsed time
    m_waveElapsedTime += Time::GetDeltaTime();
    
    // Handle spawning
    if (m_currentSpawnIndex < m_currentSpawnPattern.size())
    {
        const SpawnData& spawnData = m_currentSpawnPattern[m_currentSpawnIndex];
        
        if (m_waveElapsedTime >= spawnData.delayTime)
        {
            SpawnHon(spawnData.honType, spawnData.x, spawnData.y);
            m_currentSpawnIndex++;
            m_lastSpawnTime = m_waveElapsedTime;
        }
    }
    
    // Check active hons positions (destroy when reaching ground)
    for (auto it = m_activeHons.begin(); it != m_activeHons.end();)
    {
        GameObject* hon = *it;
        if (!hon || !hon->IsActive())
        {
            it = m_activeHons.erase(it);
            m_destroyedCount++;
            continue;
        }
        
        Transform* transform = hon->GetComponent<Transform>();
        if (transform)
        {
            Vector2 pos = transform->GetPosition();
            // Check if reached ground (y < -400)
            if (pos.y < -400.0f)
            {
                hon->SetActive(false);
                it = m_activeHons.erase(it);
                m_destroyedCount++;
                
                char debugMsg[256];
                sprintf_s(debugMsg, "Hon reached ground. Active: %d, Destroyed: %d/%d\n", 
                    static_cast<int>(m_activeHons.size()), m_destroyedCount, m_spawnedCount);
                OutputDebugStringA(debugMsg);
                continue;
            }
        }
        ++it;
    }
    
    // Check wave completion
    if (m_waveElapsedTime >= m_waveDuration)
    {
        char debugMsg[256];
        sprintf_s(debugMsg, "Wave %d time expired! Notifying GameManager.\n", static_cast<int>(m_currentWaveState));
        OutputDebugStringA(debugMsg);
        
        // Clean up remaining hons
        for (auto* hon : m_activeHons)
        {
            if (hon && hon->IsActive())
            {
                hon->SetActive(false);
            }
        }
        m_activeHons.clear();
        
        // Notify GameManager that wave is complete
        m_gameManager->isWave = false;
        m_currentWaveState = WaveState::IDLE;
        m_waveStartCheck = true;
        
        OutputDebugStringA("Wave ended. Waiting for GameManager to start next wave.\n");
    }
    
    // Send state to GameManager
    if (m_gameManager)
    {
        m_gameManager->waveTime = m_waveDuration - m_waveElapsedTime; // 남은 시간 전달
        
        // Debug GameManager communication every 2 seconds
        static float debugTimer = 0.0f;
        debugTimer += Time::GetDeltaTime();
        if (debugTimer >= 2.0f)
        {
            char debugMsg[512];
            sprintf_s(debugMsg, "GameManager Update - isWave: %s, waveTime: %.1f, waveCount: %d, honCount: %d\n",
                m_gameManager->isWave ? "true" : "false",
                m_gameManager->waveTime,
                m_gameManager->waveCount,
                m_gameManager->honCount);
            OutputDebugStringA(debugMsg);
            debugTimer = 0.0f;
        }
    }
}

void WaveSystem::StartWave(int waveNumber)
{
    StopWave();
    
    switch (waveNumber)
    {
    case 1:
        m_currentWaveState = WaveState::WAVE_1;
        m_waveDuration = 2.0f;        
        SetupWave1Pattern();
        OutputDebugStringA("Wave 1 Started - Tutorial (HonA, HonB)\n");
        break;
        
    case 2:
        m_currentWaveState = WaveState::WAVE_2;
        m_waveDuration = 2.0f;        
        SetupWave2Pattern();
        OutputDebugStringA("Wave 2 Started - Chain Reaction (HonA, HonB, HonC)\n");
        break;
        
    case 3:
        m_currentWaveState = WaveState::WAVE_3;
        m_waveDuration = 2.0f;        
        SetupWave3Pattern();
        OutputDebugStringA("Wave 3 Started - Increased Difficulty (All Hons)\n");
        break;
        
    case 4:
        m_currentWaveState = WaveState::WAVE_BOSS;
        m_waveDuration = 80.0f;        
        SetupBossPattern();
        OutputDebugStringA("Boss Wave Started!\n");
        break;
        
    default:
        OutputDebugStringA("Invalid wave number!\n");
        return;
    }
    
    m_waveElapsedTime = 0.0f;
    m_currentSpawnIndex = 0;
    m_lastSpawnTime = 0.0f;
    m_spawnedCount = 0;
    m_destroyedCount = 0;
}

void WaveSystem::StopWave()
{
    // Clean up active hons
    for (auto* hon : m_activeHons)
    {
        if (hon && hon->IsActive())
        {
            hon->SetActive(false);
        }
    }
    m_activeHons.clear();
    
    // Clean up boss
    if (m_activeBoss && m_activeBoss->IsActive())
    {
        m_activeBoss->SetActive(false);
        m_activeBoss = nullptr;
    }
    
    m_currentWaveState = WaveState::IDLE;
    m_currentSpawnPattern.clear();
}

void WaveSystem::ResetWaveSystem()
{
    StopWave();
    m_waveElapsedTime = 0.0f;
    m_waveDuration = 0.0f;
    m_currentSpawnIndex = 0;
    m_lastSpawnTime = 0.0f;
    m_spawnedCount = 0;
    m_destroyedCount = 0;
}

void WaveSystem::OnHonDestroyed(GameObject* hon)
{
    if (!hon) return;
    
    // Remove from active list (only if exists)
    auto it = std::find(m_activeHons.begin(), m_activeHons.end(), hon);
    if (it != m_activeHons.end())
    {
        m_activeHons.erase(it);
        m_destroyedCount++;
        
        char debugMsg[256];
        sprintf_s(debugMsg, "Hon manually destroyed. Active: %d, Destroyed: %d/%d\n", 
            static_cast<int>(m_activeHons.size()), m_destroyedCount, m_spawnedCount);
        OutputDebugStringA(debugMsg);
    }
}

void WaveSystem::SetupWave1Pattern()
{
    m_currentSpawnPattern.clear();
    
    // Wave 1: Tutorial - low density HonA and HonB only
    float spawnInterval = 3.5f;
    
    for (int i = 0; i < 20; i++)
    {
        SpawnData data;
        data.x = GetRandomSpawnX();
        data.y = SPAWN_Y;
        data.honType = (i % 2 == 0) ? 0 : 1;  // Alternate HonA and HonB
        data.delayTime = i * spawnInterval;
        
        m_currentSpawnPattern.push_back(data);
    }
}

void WaveSystem::SetupWave2Pattern()
{
    m_currentSpawnPattern.clear();
    
    // Wave 2: Chain utilization - add HonC for increased chain reactions
    float spawnInterval = 2.8f;
    
    for (int i = 0; i < 25; i++)
    {
        SpawnData data;
        data.x = GetRandomSpawnX();
        data.y = SPAWN_Y;
        
        // HonA(40%), HonB(40%), HonC(20%)
        int rand = m_randomGen() % 10;
        if (rand < 4)
            data.honType = 0;  // HonA
        else if (rand < 8)
            data.honType = 1;  // HonB
        else
            data.honType = 2;  // HonC
            
        data.delayTime = i * spawnInterval;
        m_currentSpawnPattern.push_back(data);
    }
}

void WaveSystem::SetupWave3Pattern()
{
    m_currentSpawnPattern.clear();
    
    // Wave 3: Increased difficulty - all hon types, high density
    float spawnInterval = 2.2f;
    
    for (int i = 0; i < 30; i++)
    {
        SpawnData data;
        data.x = GetRandomSpawnX();
        data.y = SPAWN_Y;
        
        // All hon types evenly distributed
        data.honType = m_randomGen() % 4;  // 0-3: HonA, HonB, HonC, HonD
        data.delayTime = i * spawnInterval;
        
        m_currentSpawnPattern.push_back(data);
    }
}

void WaveSystem::SetupBossPattern()
{
    m_currentSpawnPattern.clear();
    
    // Boss wave: 1 boss + continuous hon spawning
    SpawnBoss();
    
    // Hons that spawn with boss
    float spawnInterval = 3.0f;
    
    for (int i = 0; i < 20; i++)
    {
        SpawnData data;
        data.x = GetRandomSpawnX();
        data.y = SPAWN_Y;
        data.honType = m_randomGen() % 4;  // All types
        data.delayTime = 5.0f + (i * spawnInterval);  // Start after 5 seconds
        
        m_currentSpawnPattern.push_back(data);
    }
}

void WaveSystem::SpawnHon(int honType, float x, float y)
{
    auto scene = SceneManager::Get().GetCurrentScene();
    if (!scene) return;
    
    GameObject* newHon = nullptr;
    
    switch (honType)
    {
    case 0:
        newHon = scene->CreateObject<HonA>();
        break;
    case 1:
        newHon = scene->CreateObject<HonB>();
        break;
    case 2:
        newHon = scene->CreateObject<HonC>();
        break;
    case 3:
        newHon = scene->CreateObject<HonD>();
        break;
    default:
        return;
    }
    
    if (newHon)
    {
        // Set position
        auto* transform = newHon->GetComponent<Transform>();
        if (transform)
        {
            transform->SetPosition(x, y);
        }
        
        // Physics setup - controllers handle descent directly
        auto* rb = newHon->GetComponent<Rigidbody>();
        if (rb)
        {
            rb->useGravity = false;
            rb->isKinematic = true;
        }
        
        m_activeHons.push_back(newHon);
        m_spawnedCount++;
        
        char debugMsg[256];
        sprintf_s(debugMsg, "Spawned Hon Type %d at (%.1f, %.1f) - Total: %d\n", 
            honType, x, y, m_spawnedCount);
        OutputDebugStringA(debugMsg);
    }
}

void WaveSystem::SpawnBoss()
{
    auto scene = SceneManager::Get().GetCurrentScene();
    if (!scene) return;
    
    m_activeBoss = scene->CreateObject<Boss>();
    
    if (m_activeBoss)
    {
        // Boss spawns at center
        auto* transform = m_activeBoss->GetComponent<Transform>();
        if (transform)
        {
            transform->SetPosition(0.0f, BOSS_SPAWN_Y);
        }
        
        // Boss physics setup
        auto* rb = m_activeBoss->GetComponent<Rigidbody>();
        if (rb)
        {
            rb->useGravity = true;
            rb->isKinematic = false;
            // Boss falls slowly
            rb->gravityScale = 0.1f;
        }
        
        OutputDebugStringA("Boss spawned at center!\n");
    }
}

float WaveSystem::GetRandomSpawnX()
{
    return m_xDistribution(m_randomGen);
}

float WaveSystem::GetSpawnY(bool isBoss)
{
    return isBoss ? BOSS_SPAWN_Y : SPAWN_Y;
}

WaveState WaveSystem::GetCurrentWaveState() const
{
    return m_currentWaveState;
}

float WaveSystem::GetWaveElapsedTime() const
{
    return m_waveElapsedTime;
}

int WaveSystem::GetSpawnedCount() const
{
    return m_spawnedCount;
}

int WaveSystem::GetDestroyedCount() const
{
    return m_destroyedCount;
}

bool WaveSystem::IsWaveActive() const
{
    return m_currentWaveState != WaveState::IDLE && m_currentWaveState != WaveState::COMPLETED;
}