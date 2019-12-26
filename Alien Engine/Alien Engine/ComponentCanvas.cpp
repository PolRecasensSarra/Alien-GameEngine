#include "ComponentCanvas.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "PanelGame.h"

ComponentCanvas::ComponentCanvas(GameObject* attached) :Component(attached)
{
	type = ComponentType::CANVAS;
	size = {100, 100}; // temp -> will be changed in inspector
}

ComponentCanvas::~ComponentCanvas()
{
}

void ComponentCanvas::Update()
{
	size = {App->ui->panel_game->width, App->ui->panel_game->height};
}

void ComponentCanvas::PostUpdate()
{
	DebugDraw();
}

void ComponentCanvas::DebugDraw()
{
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);


	glBegin(GL_LINE_LOOP);
	glColor4f(actual_color.x, actual_color.y, actual_color.z, actual_color.w);

	float3 pos = transform->GetGlobalPosition();
	float3 size_mult = transform->GetGlobalScale();

	float3 v1 = float3(pos.x, pos.y, pos.z);
	float3 v2 = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	float3 v3 = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);
	float3 v4 = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	

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
	to_save->SetColor("Color", { actual_color.x,actual_color.y,actual_color.z ,actual_color.w });
}

void ComponentCanvas::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
	Color c = to_load->GetColor("Color");
	actual_color = { c.r, c.g,c.b, c.a };
}

bool ComponentCanvas::FadeAllUIElements(GameObject* gameObject)
{
	bool ret = false;
	if (!gameObject->components.empty()) {
		std::vector<Component*>::iterator item = gameObject->components.begin();
		for (; item != gameObject->components.end(); ++item) {
			if (*item != nullptr && (*item)->IsEnabled()) {
				ret = (*item)->Fade();
			}
		}
	}

	if (!gameObject->children.empty()) {
		std::vector<GameObject*>::iterator child = gameObject->children.begin();
		for (; child != gameObject->children.end(); ++child) {
			if (*child != nullptr && (*child)->IsEnabled()) {
				FadeAllUIElements((*child));
			}
		}
	}
	if (faded)
		ret = true;

	return ret;
}

bool ComponentCanvas::Fade()
{
	if (actual_color.w <= 0.01)
	{
		game_object_attached->enabled = false;
		faded = true;
		return true;
	}
	else
	{
		actual_color.w -= 0.01;
		return false;
	}
}
