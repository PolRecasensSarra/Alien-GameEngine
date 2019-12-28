#pragma once
#define MAX_CHARACTERS 10

#include "Component.h"
#include "imgui/imgui.h"
#include "ModuleImporter.h"


enum class InteractiveStates;

class ComponentInputText : public Component {
	friend class CompZ;

public:
	ComponentInputText(GameObject* attach, float2 size = { 0.0f,0.0f }, bool is_custom = false);
	~ComponentInputText();

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

	void CreateInputTextPlane();
	void UpdateInputTextPlane();

	void BindTex();
	bool Fade();
	void AddText(const char* txt);
	void DeleteText();

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

	float2 size_input_text;
	bool dragable = false;
	bool function = false;
	bool is_custom = false;
	bool fading = false;

	ImVec4 actual_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 normal_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 hover_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	InteractiveStates state = InteractiveStates::NO_STATE;

	std::string name_input;

public:
	ResourceTexture* tex = nullptr;
	//ComponentLabel* label = nullptr;
};