#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"

class ComponentImage : public Component {
	friend class CompZ;

public:

	ComponentImage(GameObject* attach, float2 position, SDL_Rect* section);
	~ComponentImage();
	void Draw(const float& dt);

private:
	float2 position;
	SDL_Rect* section = nullptr;
};
