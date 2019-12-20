#include "ComponentImage.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"
#include "ResourceTexture.h"
ComponentImage::ComponentImage(GameObject* attach, float2 position):Component(attach)
{
	type = ComponentType::IMAGE;
	this->position = position;

}

ComponentImage::~ComponentImage()
{
	if (texture != nullptr)
		texture->DecreaseReferences();
}

void ComponentImage::Draw(const float& dt)
{
	//draw the UI
}

void ComponentImage::BindImg()
{
	if (texture != nullptr && texture->id > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		
	}
}

const ResourceTexture* ComponentImage::GetTexture() const
{
	return texture;
}

void ComponentImage::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentImage* img = (ComponentImage*)component;
		if (texture != nullptr)
			texture->DecreaseReferences();
		texture = img->texture;
		if (texture != nullptr)
			texture->IncreaseReferences();

		//color = material->color;
	}
}
