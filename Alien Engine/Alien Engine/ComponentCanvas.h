#pragma once

#include "Component.h"

class ComponentUI;

class ComponentCanvas :public Component
{

public:
	ComponentCanvas(GameObject* attached);
	~ComponentCanvas();

	void Update();
	void PostUpdate();
	void DebugDraw();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);


public: 
	float2 size;
};