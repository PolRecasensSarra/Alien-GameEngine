#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "ModuleImporter.h"
#include <map>

struct Character;

struct LabelLetter {
	char letter = NULL;
	uint texture_id = 0;
	//ComponentTransform* rect = nullptr;

};
class ComponentLabel :public Component 
{
	friend class CompZ;

public:
	ComponentLabel(GameObject* attach, float2 size);
	~ComponentLabel();

	void PostUpdate();
	void Draw();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void CreateTextPlane();

	void SetResourceFont(ResourceFont* r_font);

	bool DrawInspector();
	const ResourceFont* GetText()const;
	void UpdateTextPlane();


private:
	uint indexId = 0;
	uint vertexId = 0;
	uint index[6]{
	0,1,2,
	2,3,0
	};

	float3 vertex[4];
	float2 size_text;
	ResourceFont* text_img = nullptr;

	bool CreateText = false;
};