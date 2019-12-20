#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"
#include "ModuleImporter.h"
class ComponentImage : public Component {
	friend class CompZ;

public:

	ComponentImage(GameObject* attach, float2 position);
	~ComponentImage();
	void Draw(const float& dt);


	void BindImg();

	const ResourceTexture* GetTexture() const;

	void SetComponent(Component* comp);
private:
	float2 position;
	ResourceTexture* texture;

};
