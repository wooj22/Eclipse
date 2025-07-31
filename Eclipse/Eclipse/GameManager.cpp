#include "GameManager.h"

void GameManager::UnInit()
{
	g_playUI = nullptr;
	//skillMap.clear();
}

void GameManager::ReSetData()
{
	honCount = 0;
	waveCount = 0;
	isWave = false;
	g_playUI = nullptr;
}