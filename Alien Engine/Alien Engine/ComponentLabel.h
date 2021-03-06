#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleImporter.h"
#include "imgui/imgui.h"
#include <map>
#include<string>


class ComponentTransform;
class Font;

struct LabelLetter {
	char letter = NULL;
	uint texture_id = 0;
	ComponentTransform* rect = nullptr;

	float2 size;
	float2 bearing;
	uint advance;

	uint indexId = 0;
	uint vertexId = 0;
	uint index[6]{
	0,1,2,
	2,3,0
	};
};

class ComponentLabel :public Component
{
	friend class CompZ;

public:
	ComponentLabel(GameObject* attach,std::string text = "example" ,float2 size = { 0.0f,0.0f }, bool is_custom = false);
	~ComponentLabel();

	void PostUpdate();
	void Draw();

	void BindText(uint texid,LabelLetter l);
	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);
	void CreateTextPlane();
	bool DrawInspector();
	void UpdateTextPlane(float sizeX, LabelLetter l);

	void CreatePPlane(LabelLetter& l);
	bool Fade();


private:

	ImVec4 actual_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	uint indexId = 0;
	uint vertexId = 0;
	uint index[6]{
	0,1,2,
	2,3,0
	};


	 
	float3 vertex[4];
	float2 uv[4];
	float2 size_text;

	bool CreateText = false;
	bool new_word = false;

	
	

public:
	std::string finalText = "example";
	bool is_custom = false;
	float3 pos;
	Font* text_font = nullptr;
	uint textureID = 0;

	std::vector<uint> texturesIDS; 

	std::vector<LabelLetter> word;

	ResourceFont* r_font = nullptr;
};