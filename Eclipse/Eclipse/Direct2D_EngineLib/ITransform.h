#pragma once
#include "Component.h"

/* Transfrom Componenets Interface */
class ITransform : public Component
{
public:
	virtual ~ITransform() = default;
	virtual void Update() = 0;
};