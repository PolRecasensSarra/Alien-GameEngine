#pragma once

#include "Component.h"
#include "imgui/imgui.h"

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
	bool FadeAllUIElements(GameObject* gameObject);
	bool Fade();

public: 
	float2 size;
	ImVec4 actual_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	bool faded = false;
};