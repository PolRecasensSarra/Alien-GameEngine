#pragma once

#include "Component.h"
#include "imgui/imgui.h"
#include "ModuleImporter.h"


class ComponentImage;
enum class InteractiveStates;

class ComponentButton : public Component {
	friend class CompZ;

public:

	ComponentButton(GameObject* attach);
	ComponentButton(GameObject* attach, float2 size);
	~ComponentButton();
	void Update();
	void PostUpdate();
	void Draw();
	void UpdateStates();
	void DoLogicClicked();
	void DoLogicHovered();
	void DoLogicPressed();
	void DoLogicExit();
	
	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	bool DrawInspector();

	void BindTex();
private:

	float2 size_button;
	bool dragable = false;
	bool function = false;

	ImVec4 actual_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 normal_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 hover_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	InteractiveStates state = InteractiveStates::NO_STATE;
	ComponentImage* image = nullptr;

	ResourceTexture* tex = nullptr;
};