#pragma once

#include "Component.h"
#include "imgui/imgui.h"


class ComponentImage;

class ComponentButton : public Component {
	friend class CompZ;

public:

	ComponentButton(GameObject* attach, float2 size);
	~ComponentButton();
	void Update();
	void PostUpdate();
	void Draw();
	void DoLogicClicked();
	void DoLogicHovered();
	void DoLogicPressed();
	void ShowInspector();


private:

	float2 size_button;

	bool function = false;

	ImVec4 normal_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 highlighted_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	ComponentImage* image = nullptr;
};