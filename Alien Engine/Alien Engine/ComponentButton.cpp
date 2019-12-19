#include "ComponentButton.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"

ComponentButton::ComponentButton(GameObject* attach, float2 size) :Component(attach)
{
	type = ComponentType::BUTTON;
	;
	this->size = size;
	this->size_button = size;

	if (size.x == 0) 
	{
		this->size.x = 20;
		this->size.y = 10;
	}
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::Update()
{
	if (function)
	{
		//call the fade function mega hardcoded
	}
	else
	{
		if (image != nullptr)
		{
			//Change the color to the normal because there are no functions with this button
		}
	}
}

void ComponentButton::PostUpdate()
{
	Draw();
}

void ComponentButton::Draw()
{
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	if (transform != nullptr) {

		glBegin(GL_LINE_LOOP);
		glLineWidth(8.0f);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		float3 pos = transform->GetGlobalPosition();

		float3 v1 = float3(pos.x, pos.y, pos.z);
		float3 v2 = float3(pos.x + size.x, pos.y, pos.z);
		float3 v3 = float3(pos.x + size.x, pos.y + size.y, pos.z);
		float3 v4 = float3(pos.x, pos.y + size.y, pos.z);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);
		

		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//do magic
}

void ComponentButton::DoLogicClicked()
{
	function = true;
}

void ComponentButton::DoLogicHovered()
{
	//change colors
}

void ComponentButton::DoLogicPressed()
{
	//change color
}

void ComponentButton::ShowInspector()
{
	//inspector magic
}


