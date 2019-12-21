#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"
#include "ModuleImporter.h"
class ComponentImage : public Component {
	friend class CompZ;

public:

	ComponentImage(GameObject* attach, float2 size);
	~ComponentImage();
	void Draw(const float& dt);


	void BindImg();

	const ResourceTexture* GetTexture() const;

	void SetComponent(Component* comp);

	void CreatImgPlane();
	void UpdateImgPlane();
	//
	void PostUpdate();
	void Draw();
	float3 vertex[4];
	float2 uv[4];
	uint index[6]{
		2,1,0,
		3,1,2
	};

	bool DrawInspector();
	uint indexId = 0;
	uint vertexId = 0;
	uint textureId = 0;


	float2 sizeIMG;
private:
	float2 position;
	ResourceTexture* texture = nullptr;
	bool createIMG = false;
};
