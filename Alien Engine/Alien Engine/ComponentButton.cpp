#include "ComponentButton.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"

ComponentButton::ComponentButton(GameObject* attach, float2 position, SDL_Rect* section) :Component(attach)
{
	type = ComponentType::BUTTON;
	this->position = position;
	this->section = section;
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::Draw(const float& dt)
{
}

void ComponentButton::DoLogicClicked(std::string& functionName)
{
}

void ComponentButton::DoLogicHovered()
{
}
