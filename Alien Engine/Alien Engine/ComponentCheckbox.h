#pragma once

#include "Component.h"
#include "imgui/imgui.h"
#include "ModuleImporter.h"


enum class InteractiveStates;
class ComponentImage;

class ComponentCheckbox : public Component {
	friend class CompZ;

public:

	ComponentCheckbox(GameObject* attach, float2 size = { 0.0f,0.0f });
	~ComponentCheckbox();
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

	void CreatCheckboxPlane();
	void UpdateCheckboxPlane();

	void BindTex();
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

	float3 vertex_check[4];
	float2 uv_check[4];
	uint index_check[6]{
		0,1,2,
		2,3,0
	};
	uint indexId_check = 0;
	uint vertexId_check = 0;
	uint textureId_check = 0;
	

	float2 size_checkbox;
	float2 size_check;
	float3 pos_check;

	bool clicked	= false;
	bool dragable	= false;
	bool function	= false;

	ImVec4 actual_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 normal_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImVec4 hover_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pressed_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	ImVec4 actual_check_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 normal_check_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 pressed_check_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

	InteractiveStates state = InteractiveStates::NO_STATE;

	ResourceTexture* tex = nullptr;
	//ResourceTexture* tex_check = nullptr;

	ComponentImage* check_image = nullptr;
};