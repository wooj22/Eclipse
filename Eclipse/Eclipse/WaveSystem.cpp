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
		m_waveDuration = 70.0f;
		SetupWave1Pattern();
		OutputDebugStringA("Wave 1 Started - Tutorial (HonA, HonB)\n");
		break;
	case 2:
		m_currentWaveState = WaveState::WAVE_2;
		m_waveDuration = 70.0f;
		SetupWave2Pattern();
		OutputDebugStringA("Wave 2 Started - Chain Reaction (HonA, HonB, HonC)\n");
		break;
	case 3:
		m_currentWaveState = WaveState::WAVE_3;
		m_waveDuration = 70.0f;
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
	float currentTime = 0.0f;

	// Pattern 1: HonA, HonB 중앙 충돌 유도 (0-15s)
	// 좌우에서 중앙으로 모여드는 패턴
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			SpawnData data;
			data.delayTime = currentTime + j * 0.3f;
			data.x = (j == 0 ? -1000.0f : 1000.0f) + (j == 0 ? i * 200.0f : -i * 200.0f);
			data.y = SPAWN_Y;
			data.honType = j;
			m_currentSpawnPattern.push_back(data);
		}
		currentTime += 3.0f;
	}

	// Pattern 2: 밀집 대형 (15-35s)
	// 좁은 영역에 여러 Hon을 배치하여 충돌 확률 증가
	for (int i = 0; i < 4; ++i)
	{
		float startX = GetRandomSpawnX() * 0.5f;
		for (int j = 0; j < 5; ++j)
		{
			SpawnData data;
			data.delayTime = currentTime + j * 0.2f;
			data.x = startX + (j - 2.0f) * 100.0f;
			data.x = (std::max)(-1200.0f, (std::min)(1200.0f, data.x));
			data.y = SPAWN_Y;
			data.honType = j % 2;
			m_currentSpawnPattern.push_back(data);
		}
		currentTime += 4.0f;
	}

	// Pattern 3: 교차 낙하 (35-55s)
	// Hon들이 교차하며 떨어져 충돌을 유도
	currentTime = 35.0f;
	for (int i = 0; i < 10; ++i)
	{
		SpawnData left, right;
		left.delayTime = currentTime;
		left.x = -800.0f + i * 150.0f;
		left.y = SPAWN_Y;
		left.honType = 0;
		m_currentSpawnPattern.push_back(left);

		right.delayTime = currentTime;
		right.x = 800.0f - i * 150.0f;
		right.y = SPAWN_Y;
		right.honType = 1;
		m_currentSpawnPattern.push_back(right);

		currentTime += 1.5f;
	}

	// Pattern 4: 최종 압박 (55-70s)
	// 여러 Hon이 동시다발적으로 떨어짐
	currentTime = 55.0f;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			SpawnData data;
			data.delayTime = currentTime + j * 0.2f;
			data.x = GetRandomSpawnX();
			data.y = SPAWN_Y;
			data.honType = j % 2;
			m_currentSpawnPattern.push_back(data);
		}
		currentTime += 3.0f;
	}
}
void WaveSystem::SetupWave2Pattern()
{
	m_currentSpawnPattern.clear();
	float currentTime = 0.0f;

	// Pattern 1: HonC를 중심으로 한 분산 패턴 (0-15s)
	// 넓은 간격으로 HonA, HonB를 배치하고, 그 중앙에 HonC를 떨어뜨립니다.
	for (int i = 0; i < 3; ++i)
	{
		float honCX = -800.0f + i * 800.0f;
		SpawnData honc;
		honc.delayTime = currentTime;
		honc.x = honCX;
		honc.y = SPAWN_Y;
		honc.honType = 2; // HonC
		m_currentSpawnPattern.push_back(honc);

		for (int j = 0; j < 4; ++j)
		{
			SpawnData data;
			data.delayTime = currentTime + 0.5f;
			data.x = honCX + (j % 2 == 0 ? 300.0f : -300.0f);
			data.y = SPAWN_Y + (j / 2) * 200.0f;
			data.honType = j % 2;
			m_currentSpawnPattern.push_back(data);
		}
		currentTime += 5.0f;
	}

	// Pattern 2: HonC와 주변 HonA, HonB (15-30s)
	// HonC가 떨어지고, 그 주변에 HonA와 HonB가 흩어져서 떨어집니다.
	currentTime = 15.0f;
	for (int i = 0; i < 4; ++i)
	{
		float honCX = GetRandomSpawnX() * 0.7f;
		SpawnData honc;
		honc.delayTime = currentTime;
		honc.x = honCX;
		honc.y = SPAWN_Y;
		honc.honType = 2; // HonC
		m_currentSpawnPattern.push_back(honc);

		for (int j = 0; j < 4; ++j)
		{
			SpawnData data;
			data.delayTime = currentTime + 0.5f + j * 0.2f;
			data.x = (std::max)(-1200.0f, (std::min)(1200.0f, honCX + (j - 2.0f) * 200.0f));
			data.y = SPAWN_Y;
			data.honType = j % 2;
			m_currentSpawnPattern.push_back(data);
		}
		currentTime += 4.0f;
	}

	// Pattern 3: HonA, HonB 물결과 HonC 징검다리 (30-55s)
	// HonA와 HonB가 좌우에서 물결처럼 쏟아지며, 그 사이에 HonC를 하나씩 배치하여 연쇄 폭발을 위한 징검다리를 놓습니다.
	currentTime = 30.0f;
	for (int i = 0; i < 5; ++i)
	{
		SpawnData honc;
		honc.delayTime = currentTime;
		honc.x = GetRandomSpawnX() * 0.5f;
		honc.y = SPAWN_Y;
		honc.honType = 2; // HonC
		m_currentSpawnPattern.push_back(honc);

		for (int j = 0; j < 3; ++j)
		{
			SpawnData left, right;
			left.delayTime = currentTime + 0.5f + j * 0.5f;
			left.x = -1000.0f + j * 100.0f;
			left.y = SPAWN_Y;
			left.honType = 0;
			m_currentSpawnPattern.push_back(left);

			right.delayTime = currentTime + 0.5f + j * 0.5f;
			right.x = 1000.0f - j * 100.0f;
			right.y = SPAWN_Y;
			right.honType = 1;
			m_currentSpawnPattern.push_back(right);
		}
		currentTime += 5.0f;
	}

	// Pattern 4: 최종 폭탄 투하 (55-70s)
	// 넓은 간격으로 HonC를 흩뿌려, 플레이어가 직접 여러 HonC를 연결하여 폭발시켜야 합니다.
	currentTime = 55.0f;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			SpawnData honc;
			honc.delayTime = currentTime + j * 0.5f;
			honc.x = -1000.0f + j * 1000.0f;
			honc.y = SPAWN_Y;
			honc.honType = 2; // HonC
			m_currentSpawnPattern.push_back(honc);
		}
		currentTime += 3.0f;
	}
}

void WaveSystem::SetupWave3Pattern()
{
	m_currentSpawnPattern.clear();
	float currentTime = 0.0f;

	// Pattern 1: HonD와 방해꾼의 등장 (0-15s)
	// HonD 한 마리가 떨어지고, HonA와 HonB가 뒤따라 떨어져 HonD를 위협합니다.
	for (int i = 0; i < 2; ++i)
	{
		float honDX = (i == 0) ? -600.0f : 600.0f;
		SpawnData honD;
		honD.delayTime = currentTime;
		honD.x = honDX;
		honD.y = SPAWN_Y;
		honD.honType = 3; // HonD
		m_currentSpawnPattern.push_back(honD);

		for (int j = 0; j < 3; ++j)
		{
			SpawnData disruptor;
			disruptor.delayTime = currentTime + 0.5f + j * 0.5f;
			disruptor.x = honDX + (j - 1.0f) * 150.0f;
			disruptor.y = SPAWN_Y;
			disruptor.honType = j % 2; // HonA, HonB
			m_currentSpawnPattern.push_back(disruptor);
		}
		currentTime += 5.0f;
	}

	// Pattern 2: HonD와 HonC의 위협 (15-30s)
	// HonD가 떨어지는 지점 근처에 HonC를 배치하여, HonC가 폭발하면 HonD도 파괴될 위험을 만듭니다.
	currentTime = 15.0f;
	for (int i = 0; i < 2; ++i)
	{
		float honDX = (i == 0) ? -400.0f : 400.0f;
		SpawnData honD;
		honD.delayTime = currentTime;
		honD.x = honDX;
		honD.y = SPAWN_Y;
		honD.honType = 3;
		m_currentSpawnPattern.push_back(honD);

		SpawnData honC;
		honC.delayTime = currentTime + 1.0f;
		honC.x = honDX + (i == 0 ? 200.0f : -200.0f);
		honC.y = SPAWN_Y;
		honC.honType = 2; // HonC
		m_currentSpawnPattern.push_back(honC);
		currentTime += 5.0f;
	}

	// Pattern 3: HonD 삼각 진형과 러시 (30-45s)
	// HonD 3마리가 삼각 진형으로 떨어지며, 주변에서 HonA와 HonB가 쏟아져 압박합니다.
	currentTime = 30.0f;
	SpawnData honD_center, honD_left, honD_right;
	honD_center.delayTime = currentTime;
	honD_center.x = 0.0f;
	honD_center.y = SPAWN_Y;
	honD_center.honType = 3;
	m_currentSpawnPattern.push_back(honD_center);

	honD_left.delayTime = currentTime + 0.5f;
	honD_left.x = -500.0f;
	honD_left.y = SPAWN_Y;
	honD_left.honType = 3;
	m_currentSpawnPattern.push_back(honD_left);

	honD_right.delayTime = currentTime + 0.5f;
	honD_right.x = 500.0f;
	honD_right.y = SPAWN_Y;
	honD_right.honType = 3;
	m_currentSpawnPattern.push_back(honD_right);

	for (int i = 0; i < 5; ++i)
	{
		SpawnData rusher;
		rusher.delayTime = currentTime + 2.0f + i * 0.3f;
		rusher.x = GetRandomSpawnX();
		rusher.y = SPAWN_Y;
		rusher.honType = i % 2; // HonA, HonB
		m_currentSpawnPattern.push_back(rusher);
	}
	currentTime += 5.0f;

	// Pattern 4: HonD와 HonC의 최종 압박 (45-70s)
	// HonD가 중앙에 떨어지고, 양 옆에서 HonC와 HonA, HonB가 동시에 쏟아집니다.
	currentTime = 45.0f;
	for (int i = 0; i < 3; ++i)
	{
		SpawnData honD;
		honD.delayTime = currentTime;
		honD.x = -400.0f + i * 400.0f;
		honD.y = SPAWN_Y;
		honD.honType = 3;
		m_currentSpawnPattern.push_back(honD);

		SpawnData honC_left, honC_right;
		honC_left.delayTime = currentTime + 1.0f;
		honC_left.x = -800.0f;
		honC_left.y = SPAWN_Y;
		honC_left.honType = 2;
		m_currentSpawnPattern.push_back(honC_left);

		honC_right.delayTime = currentTime + 1.0f;
		honC_right.x = 800.0f;
		honC_right.y = SPAWN_Y;
		honC_right.honType = 2;
		m_currentSpawnPattern.push_back(honC_right);

		currentTime += 4.0f;
	}
}

void WaveSystem::SetupBossPattern()
{
	m_currentSpawnPattern.clear();
	float currentTime = 0.0f;

	// Boss Spawns
	SpawnBoss();

	// Pattern 1: Boss Guards (5-15s)
	// HonD form a protective circle around the boss, with HonA and HonB flanking.
	currentTime = 5.0f;
	for (int i = 0; i < 6; ++i)
	{
		float angle = (i / 6.0f) * 2 * 3.14159f;
		float radius = 500.0f;
		SpawnData data;
		data.delayTime = currentTime + i * 0.3f;
		data.x = (std::max)(-1200.0f, (std::min)(1200.0f, cos(angle) * radius));
		data.y = SPAWN_Y;
		data.honType = 3; // HonD guards
		m_currentSpawnPattern.push_back(data);
	}

	currentTime += 2.0f;
	for (int i = 0; i < 4; ++i)
	{
		SpawnData flank;
		flank.delayTime = currentTime + i * 0.5f;
		flank.x = (i % 2 == 0 ? -1000.0f : 1000.0f);
		flank.y = SPAWN_Y;
		flank.honType = (i % 2 == 0) ? 0 : 1; // HonA and HonB
		m_currentSpawnPattern.push_back(flank);
	}
	currentTime += 5.0f;

	// Pattern 2: HonC Bombardment (15-30s)
	// Boss spawns waves of HonC, creating a dense field of explosives.
	currentTime = 15.0f;
	for (int wave = 0; wave < 3; ++wave)
	{
		// HonC are dropped in a line, creating a fuse.
		for (int i = 0; i < 5; ++i)
		{
			SpawnData honc;
			honc.delayTime = currentTime + i * 0.2f;
			honc.x = -800.0f + i * 400.0f;
			honc.y = SPAWN_Y;
			honc.honType = 2; // HonC
			m_currentSpawnPattern.push_back(honc);
		}

		currentTime += 2.0f;
		// A single HonA is dropped to detonate the line.
		SpawnData trigger;
		trigger.delayTime = currentTime;
		trigger.x = -800.0f;
		trigger.y = SPAWN_Y;
		trigger.honType = 0; // HonA
		m_currentSpawnPattern.push_back(trigger);

		currentTime += 4.0f;
	}

	// Pattern 3: HonD Walls and HonA/B Rush (30-50s)
	// The boss builds walls of HonD to block shots, while a rapid succession of HonA/B stream down.
	currentTime = 30.0f;
	for (int wave = 0; wave < 3; ++wave)
	{
		// HonD walls on the sides
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				SpawnData wall;
				wall.delayTime = currentTime + j * 0.3f;
				wall.x = (i == 0 ? -1000.0f : 1000.0f) + (i == 0 ? j * 150.0f : -j * 150.0f);
				wall.y = SPAWN_Y;
				wall.honType = 3; // HonD
				m_currentSpawnPattern.push_back(wall);
			}
		}
		currentTime += 1.0f;

		// HonA/B rush through the center
		for (int i = 0; i < 5; ++i)
		{
			SpawnData rusher;
			rusher.delayTime = currentTime + i * 0.4f;
			rusher.x = -200.0f + i * 100.0f;
			rusher.y = SPAWN_Y;
			rusher.honType = m_randomGen() % 2;
			m_currentSpawnPattern.push_back(rusher);
		}
		currentTime += 5.0f;
	}

	// Pattern 4: Final Barrage (50-75s)
	// All enemy types are used in a dense, overwhelming final push.
	currentTime = 50.0f;
	for (int i = 0; i < 5; ++i)
	{
		// HonD cluster
		for (int j = 0; j < 3; ++j)
		{
			SpawnData honD;
			honD.delayTime = currentTime;
			honD.x = -600.0f + j * 600.0f;
			honD.y = SPAWN_Y;
			honD.honType = 3;
			m_currentSpawnPattern.push_back(honD);
		}
		currentTime += 1.0f;

		// HonC cluster
		for (int j = 0; j < 2; ++j)
		{
			SpawnData honC;
			honC.delayTime = currentTime;
			honC.x = -400.0f + j * 800.0f;
			honC.y = SPAWN_Y;
			honC.honType = 2;
			m_currentSpawnPattern.push_back(honC);
		}
		currentTime += 1.0f;

		// HonA/B filler
		for (int j = 0; j < 5; ++j)
		{
			SpawnData honAB;
			honAB.delayTime = currentTime + j * 0.2f;
			honAB.x = GetRandomSpawnX();
			honAB.y = SPAWN_Y;
			honAB.honType = m_randomGen() % 2;
			m_currentSpawnPattern.push_back(honAB);
		}
		currentTime += 4.0f;
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