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

	if (!game_object_attached->HasComponent(ComponentType::TRANSFORM))
	{
		game_object_attached->AddComponent(new ComponentTransform(game_object_attached, { 0.0f,0.0f,0.0f }, { 0,0,0,0 }, { 1,1,1 }));
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
