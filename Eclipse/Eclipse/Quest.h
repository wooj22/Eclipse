#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "GameManager.h"
#include "PlayUI.h"

class Quest : public Script
{
private:
	std::vector<std::wstring> questLines;
public:
	void Awake() override;
	void Update() override;
};

