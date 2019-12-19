#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"


class ComponentButton : public Component {
	friend class CompZ;

public:

	ComponentButton(GameObject* attach, float2 position);
	~ComponentButton();
	void Draw(const float& dt);


private:
	float2 position;

};