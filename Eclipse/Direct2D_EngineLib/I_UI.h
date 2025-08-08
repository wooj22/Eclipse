#pragma once
#include "Component.h"

/* UI Componenets Interface */
class I_UI : public Component
{
public:
	virtual ~I_UI() = default;
	virtual void Update() = 0;
};