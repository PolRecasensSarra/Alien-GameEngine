#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL.h"

class ComponentButton : public Component {
	friend class CompZ;

public:

	ComponentButton(GameObject* attach, float2 position, SDL_Rect* section);
	~ComponentButton();
	void Draw(const float& dt);
	void DoLogicClicked(std::string& functionName);
	void DoLogicHovered();

private:
	float2 position;
	SDL_Rect* section = nullptr;
};