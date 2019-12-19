#pragma once

#include "Component.h"

class ComponentUI;

class ComponentCanvas :public Component
{

public:
	ComponentCanvas(GameObject* attached);
	~ComponentCanvas();

	void PostUpdate();
	void DebugDraw();
};