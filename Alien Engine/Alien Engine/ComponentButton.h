#pragma once

#include "Component.h"
#include "imgui/imgui.h"
#include "ModuleImporter.h"


enum class InteractiveStates;

class ComponentButton : public Component {
	friend class CompZ;

public:

	ComponentButton(GameObject* attach);
	ComponentButton(GameObject* attach, float2 size, bool is_custom = false);
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

	void CreatButtonPlane();
	void UpdateButtonPlane();

	void BindTex();

private:
	void CheckIfDefaulTextureIsSettedAfterReturnZ();

private:

	float3 vertex[4];
	float2 uv[4];
	uint index[6]{
		0,1,2,
		2,3,0
	};
	uint indexId = 0;
	uint vertexId = 0;
	uint textureId = 0;
	bool createButtonIMG = false;

	float2 size_button;
	bool dragable = false;
	bool function = false;
	bool is_custom = false;

	ImVec4 actual_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 normal_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 hover_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	InteractiveStates state = InteractiveStates::NO_STATE;

public:
	ResourceTexture* tex = nullptr;
};