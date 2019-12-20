#include "ComponentCanvas.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleInput.h"

ComponentCanvas::ComponentCanvas(GameObject* attached) :Component(attached)
{
	type = ComponentType::CANVAS;
}

ComponentCanvas::~ComponentCanvas()
{
}

void ComponentCanvas::PostUpdate()
{
	DebugDraw();
}

void ComponentCanvas::DebugDraw()
{
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);


	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	float3 pos = transform->GetGlobalPosition();

	float3 v1 = float3(pos.x, pos.y, pos.z);
	float3 v2 = float3(pos.x + 40, pos.y, pos.z);
	float3 v3 = float3(pos.x, pos.y + 40, pos.z);
	float3 v4 = float3(pos.x + 40, pos.y + 40, pos.z);

	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v4.x, v4.y, v4.z);
	glVertex3f(v3.x, v3.y, v3.z);

	glEnd();
}

void ComponentCanvas::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetString("ID", std::to_string(ID));
}

void ComponentCanvas::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
}
