#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "ModuleImporter.h"
class ComponentImage : public Component {
	friend class CompZ;

public:

	ComponentImage(GameObject* attach, float2 size = { 0.0f,0.0f }, float3 margin = { 0.0f,0.0f,0.0f }, bool is_custom = false);
	~ComponentImage();

	void BindImg();

	const ResourceTexture* GetTexture() const;

	void SetComponent(Component* comp);

	void CreatImgPlane();
	void UpdateImgPlane();
	//
	void PostUpdate();
	void Draw();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	bool DrawInspector();
	bool Fade();

private:
	void CheckIfDefaulTextureIsSettedAfterReturnZ(); 

public:
	float3 vertex[4];
	float2 uv[4];
	uint index[6]{
		0,1,2,
		2,3,0
	};
	uint indexId = 0;
	uint vertexId = 0;
	uint textureId = 0;

	float2 sizeIMG;
	float3 margin;

	ResourceTexture* texture = nullptr;

	ImVec4 actual_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

private:
	float2 position;
	
	bool createIMG = false;
public:
	bool is_custom = false;
};
