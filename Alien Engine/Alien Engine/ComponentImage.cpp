#include "ComponentImage.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"


ComponentImage::ComponentImage(GameObject* attach, float2 position, SDL_Rect* section):Component(attach)
{
	type = ComponentType::IMAGE;
	this->position = position;
	this->section = section;

}

ComponentImage::~ComponentImage()
{
}

void ComponentImage::Draw(const float& dt)
{
	//draw the UI
}
