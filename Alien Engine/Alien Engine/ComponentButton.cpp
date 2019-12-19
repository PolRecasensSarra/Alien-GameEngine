#include "ComponentButton.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"

ComponentButton::ComponentButton(GameObject* attach, float2 position) :Component(attach)
{
	type = ComponentType::BUTTON;
	this->position = position;
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::Draw(const float& dt)
{
}


