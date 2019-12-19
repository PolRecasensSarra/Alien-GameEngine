#pragma once

#include "Component.h"

class ComponentTransform2D :public Component
{
public:
	ComponentTransform2D(GameObject* attached);
	~ComponentTransform2D();
};