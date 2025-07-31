#pragma once
#include "../Direct2D_EngineLib/Singleton.h"

class PlayUI;

class GameManager : public Singleton<GameManager>
{

public:
	PlayUI* g_playUI;
	int honCount;
	int waveCount;
	bool isWave;

	GameManager() = default;
	~GameManager() = default;

	void Init() { ReSetData(); }
	void UnInit();
	void ReSetData();
};