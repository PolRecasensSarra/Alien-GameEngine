#include "ComponentLabel.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"

ComponentLabel::ComponentLabel(GameObject* attach, float2 size) :Component(attach)
{
	type = ComponentType::LABEL;
	this->size = size;
	this->size_text = size;
	if (size.x == 0)
	{
		this->size.x = 20;
		this->size.y = 10;
	}
}

ComponentLabel::~ComponentLabel()
{
}

void ComponentLabel::Update()
{
}

void ComponentLabel::PostUpdate()
{
	Draw();
}

void ComponentLabel::Draw()
{
}

void ComponentLabel::SaveComponent(JSONArraypack* to_save)
{
}

void ComponentLabel::LoadComponent(JSONArraypack* to_load)
{
}

bool ComponentLabel::DrawInspector()
{
	return false;
}
