#pragma once

#include "Component.h"

class ComponentLabel :public Component
{
public:
	ComponentLabel(GameObject* attached);
	~ComponentLabel();
};