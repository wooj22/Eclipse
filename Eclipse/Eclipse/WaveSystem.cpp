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
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/AudioSystem.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include "PlayUI.h"
#include "Chat.h"

// Define constants
const float SPAWN_MARGIN = 50.0f;
const float SPAWN_Y = -800.0f;
const float BOSS_SPAWN_Y = -600.0f;

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
	m_waveEndAudioSource = nullptr;

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	m_randomGen = std::mt19937(static_cast<unsigned int>(seed));
	m_xDistribution = std::uniform_real_distribution<float>(-1280.0f + SPAWN_MARGIN, 1280.0f - SPAWN_MARGIN);
}

WaveSystem::~WaveSystem()
{
}

void WaveSystem::Awake()
{
	m_gameManager = &GameManager::Get();
}

void WaveSystem::Start()
{
	m_waveEndAudioSource = gameObject->AddComponent<AudioSource>();
	OutputDebugStringA("[WaveSystem] AudioSource component added.\n");

	m_waveEndAudioSource->SetChannelGroup(AudioSystem::Get().GetSFXGroup());
	OutputDebugStringA("[WaveSystem] Audio source set to SFX group.\n");

	auto waveEndClip = ResourceManager::Get().CreateAudioClip("../Resource/Aron/BGM/s_WaveEnd.wav");
	if (waveEndClip)
	{
		m_waveEndAudioSource->SetClip(waveEndClip);
		OutputDebugStringA("[WaveSystem] Wave end audio clip loaded successfully.\n");
	}
	else
	{
		OutputDebugStringA("[WaveSystem] ERROR: Failed to load wave end audio clip!\n");
	}

	m_waveEndAudioSource->SetVolume(1.0f);
	OutputDebugStringA("[WaveSystem] Wave end audio source initialized.\n");
}

void WaveSystem::Update()
{
	if (!m_gameManager->isWave)
	{
		if (m_currentWaveState != WaveState::IDLE)
		{
			m_currentWaveState = WaveState::IDLE;
			m_waveStartCheck = true;
		}
		return;
	}

	if (m_waveStartCheck && m_gameManager->isWave)
	{
		StartWave(m_gameManager->waveCount);
		m_waveStartCheck = false;
	}

	m_waveElapsedTime += Time::GetDeltaTime();

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

	for (int i = static_cast<int>(m_activeHons.size()) - 1; i >= 0; --i)
	{
		GameObject* hon = m_activeHons[i];
		if (!hon || !hon->IsActive())
		{
			m_activeHons.erase(m_activeHons.begin() + i);
			m_destroyedCount++;
			continue;
		}
	}

	if (m_waveElapsedTime >= m_waveDuration)
	{
		char debugMsg[256];
		sprintf_s(debugMsg, "Wave %d time expired! Notifying GameManager.\n", static_cast<int>(m_currentWaveState));
		OutputDebugStringA(debugMsg);

		OutputDebugStringA("Starting wave cleanup...\n");
		m_activeHons.clear();
		auto hons = GameObject::FindAllWithTag("Hon");
		for (auto* hon : hons)
		{
			if (hon && hon->IsActive())
			{
				hon->SetActive(false);
			}
		}
		OutputDebugStringA("Wave cleanup completed.\n");

		AudioSystem::Get().GetBGMGroup()->stop();
		OutputDebugStringA("[WaveSystem] All BGM stopped.\n");

		if (m_waveEndAudioSource)
		{
			OutputDebugStringA("[WaveSystem] Attempting to play wave end sound...\n");
			m_waveEndAudioSource->PlayOneShot();
			OutputDebugStringA("[WaveSystem] Wave end sound PlayOneShot() called.\n");
		}
		else
		{
		}

		if (m_gameManager)
		{
			m_gameManager->isWave = false;
			m_gameManager->FinishWave();
		}
		m_currentWaveState = WaveState::IDLE;
		m_waveStartCheck = true;
		OutputDebugStringA("Wave ended. Waiting for GameManager to start next wave.\n");
	}

	if (m_gameManager)
	{
		m_gameManager->waveTime = m_waveDuration - m_waveElapsedTime;
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
		m_waveDuration = 3.0f;
		SetupWave1Pattern();
		OutputDebugStringA("Wave 1 Started - Tutorial (HonA, HonB)\n");
		break;
	case 2:
		m_currentWaveState = WaveState::WAVE_2;
		m_waveDuration = 3.0f;
		SetupWave2Pattern();
		OutputDebugStringA("Wave 2 Started - Chain Reaction (HonA, HonB, HonC)\n");
		break;
	case 3:
		m_currentWaveState = WaveState::WAVE_3;
		m_waveDuration = 3.0f;
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
	m_activeHons.clear();
	auto hons = GameObject::FindAllWithTag("Hon");
	for (auto* hon : hons)
	{
		if (hon && hon->IsActive())
		{
			hon->SetActive(false);
		}
	}

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

	// Wave 1: 2-3초마다 1-2마리 랜덤 스폰
	float currentTime = 0.0f;
	std::uniform_real_distribution<float> intervalDist(2.0f, 3.0f);
	std::uniform_int_distribution<int> countDist(1, 2);

	while (currentTime < 70.0f)
	{
		float spawnInterval = intervalDist(m_randomGen);  // 2-3초 랜덤
		int spawnCount = countDist(m_randomGen);  // 1-2마리 랜덤

		currentTime += spawnInterval;
		if (currentTime >= 70.0f) break;

		for (int j = 0; j < spawnCount; j++)
		{
			SpawnData data;
			data.x = GetRandomSpawnX(); // 랜덤 위치 사용
			data.y = SPAWN_Y;
			data.honType = (m_randomGen() % 2 == 0) ? 0 : 1;  // A, B 랜덤
			data.delayTime = currentTime + (j * 0.2f);  // 동시 스폰시 약간의 지연

			m_currentSpawnPattern.push_back(data);
		}
	}
}
void WaveSystem::SetupWave2Pattern()
{
	m_currentSpawnPattern.clear();

	// Wave 2: 2-3초마다 1-2마리 랜덤 스폰 (A, B, C)
	float currentTime = 0.0f;
	std::uniform_real_distribution<float> intervalDist(2.0f, 3.0f);
	std::uniform_int_distribution<int> countDist(1, 2);

	while (currentTime < 70.0f)
	{
		float spawnInterval = intervalDist(m_randomGen);  // 2-3초 랜덤
		int spawnCount = countDist(m_randomGen);  // 1-2마리 랜덤

		currentTime += spawnInterval;
		if (currentTime >= 70.0f) break;

		for (int j = 0; j < spawnCount; j++)
		{
			SpawnData data;
			data.x = GetRandomSpawnX(); // 랜덤 위치 사용
			data.y = SPAWN_Y;
			data.honType = m_randomGen() % 3;  // A, B, C 랜덤
			data.delayTime = currentTime + (j * 0.2f);  // 동시 스폰시 약간의 지연

			m_currentSpawnPattern.push_back(data);
		}
	}
}

void WaveSystem::SetupWave3Pattern()
{
	m_currentSpawnPattern.clear();

	// Wave 3: 2-3초마다 2-3마리 랜덤 스폰 (모든 타입)
	float currentTime = 0.0f;
	std::uniform_real_distribution<float> intervalDist(2.0f, 3.0f);
	std::uniform_int_distribution<int> countDist(2, 3);

	while (currentTime < 70.0f)
	{
		float spawnInterval = intervalDist(m_randomGen);  // 2-3초 랜덤
		int spawnCount = countDist(m_randomGen);  // 2-3마리 랜덤

		currentTime += spawnInterval;
		if (currentTime >= 70.0f) break;

		for (int j = 0; j < spawnCount; j++)
		{
			SpawnData data;
			data.x = GetRandomSpawnX(); // 랜덤 위치 사용
			data.y = SPAWN_Y;

			// 모든 타입 포함한 랜덤 패턴 (D는 25% 확률)
			int randomNum = m_randomGen() % 100;
			if (randomNum < 25) {
				data.honType = 3;  // HonD (25% 비율)
			}
			else {
				data.honType = (randomNum - 25) % 3;  // HonA, HonB, HonC (75% 비율)
			}

			data.delayTime = currentTime + (j * 0.2f);  // 동시 스폰시 약간의 지연

			m_currentSpawnPattern.push_back(data);
		}
	}
}

void WaveSystem::SetupBossPattern()
{
	m_currentSpawnPattern.clear();

	// Boss wave: 1 boss + continuous hon spawning
	SpawnBoss();

	// Boss Pattern: 2-3초마다 2-3마리 랜덤 스폰
	float currentTime = 5.0f;  // Start after 5 seconds
	float bossWidth = 400.0f;  // 보스 크기 고려한 안전 거리
	std::uniform_real_distribution<float> intervalDist(2.0f, 3.0f);
	std::uniform_int_distribution<int> countDist(2, 3);

	while (currentTime < 80.0f)
	{
		float spawnInterval = intervalDist(m_randomGen);  // 2-3초 랜덤
		int spawnCount = countDist(m_randomGen);  // 2-3마리 랜덤

		currentTime += spawnInterval;
		if (currentTime >= 80.0f) break;

		for (int j = 0; j < spawnCount; j++)
		{
			SpawnData data;

			// 보스를 피해 좌우로만 스폰
			float randomX = GetRandomSpawnX();
			if (std::abs(randomX) < bossWidth)
			{
				// 중앙 근처면 좌우 끝으로 이동
				data.x = (randomX >= 0) ? bossWidth : -bossWidth;
			}
			else
			{
				data.x = randomX;
			}

			data.y = SPAWN_Y;

			// 0.8:1:1:1 비율로 설정 (총 3.8)
			// HonD: 0.8/3.8 = 21%, A,B,C: 1/3.8 = 26.3%씩
			int randomNum = m_randomGen() % 100;
			if (randomNum < 21)  // 21% 확률로 HonD
			{
				data.honType = 3;  // HonD
			}
			else
			{
				// 나머지 79%를 A, B, C가 균등 분배 (각각 26.3%)
				int honType = (randomNum - 21) % 3;
				data.honType = honType;  // 0: HonA, 1: HonB, 2: HonC
			}

			data.delayTime = currentTime + (j * 0.2f);  // 동시 스폰시 약간의 지연

			m_currentSpawnPattern.push_back(data);
		}
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
		auto* transform = newHon->GetComponent<Transform>();
		if (transform)
		{
			transform->SetPosition(x, y);
		}
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
		auto* transform = m_activeBoss->GetComponent<Transform>();
		if (transform)
		{
			transform->SetPosition(0.0f, BOSS_SPAWN_Y);
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