#pragma once

#include "Component.h"

class ComponentLabel :public Component
{
public:
	ComponentLabel(GameObject* attach, float2 size);
	~ComponentLabel();

	void Update();
	void PostUpdate();
	void Draw();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	bool DrawInspector();

private:
	float2 size_text;
};