#include "ComponentImage.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"


ComponentImage::ComponentImage(GameObject* attach, float2 position):Component(attach)
{
	type = ComponentType::IMAGE;
	this->position = position;


}

ComponentImage::~ComponentImage()
{
}

void ComponentImage::Draw(const float& dt)
{
	//draw the UI
}
