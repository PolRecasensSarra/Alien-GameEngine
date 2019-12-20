#include "ComponentButton.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleUI.h"
#include "PanelGame.h"
#include "ReturnZ.h"
#include "ResourceTexture.h"
ComponentButton::ComponentButton(GameObject* attach) : Component(attach)
{
	type = ComponentType::BUTTON;

}

ComponentButton::ComponentButton(GameObject* attach, float2 size) :Component(attach)
{
	type = ComponentType::BUTTON;
	
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
	UpdateStates();

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
	//TODO::// if texture exists, do bind



	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	if (transform != nullptr) {

		glBegin(GL_QUADS);
		glLineWidth(8.0f);
		glColor4f(actual_color.x, actual_color.y, actual_color.z, actual_color.w);
		
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

void ComponentButton::UpdateStates()
{
	float3 pos = float3::zero;
	
	float2 origin = float2((App->input->GetMousePosition().x - App->ui->panel_game->posX), (App->input->GetMousePosition().y - (App->ui->panel_game->posY+19)));

	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	if (transform != nullptr)
		pos = transform->GetGlobalPosition();


	float width = size_button.x;
	float height = size_button.y;
	float x = pos.x;
	float y = pos.y;

	float right, left, top, bottom;
	left = x;
	right = x + width;
	top = y;
	bottom = y + height;

	LOG("%f", origin.x);
	LOG("%f", origin.y);


	if (origin.x >= left && origin.y >= top && origin.x <= right && origin.y <= bottom)
	{

		if (state == InteractiveStates::NO_STATE)
		{
			state = InteractiveStates::ENTER;
			//Enter();
		}
		if (state == InteractiveStates::ENTER)
		{
			state = InteractiveStates::HOVER;
		}

		if (state == InteractiveStates::HOVER)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				DoLogicPressed();
			}
			else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				DoLogicPressed();
				if (dragable)
				{
					//Move(); //I don't know if this is okai so maybe we delete this
				}
			}
			else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (!dragable)
				{
					DoLogicClicked();
				}
			}
			else
			{
				DoLogicHovered();
			}
		}
	}
	else
	{
		if (state == InteractiveStates::HOVER)
		{
			state = InteractiveStates::EXIT;
			DoLogicExit();
			//Exit();
		}
		else
		{
			state = InteractiveStates::NO_STATE;
			//Idle();
		}
	}
}

void ComponentButton::DoLogicClicked()
{
	function = true;
}

void ComponentButton::DoLogicHovered()
{

	actual_color = hover_color;
	//change colors
}

void ComponentButton::DoLogicPressed()
{
	actual_color = pressed_color;
	//change color
}

void ComponentButton::DoLogicExit()
{
	actual_color = normal_color;
}

void ComponentButton::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetFloat2("SizeButton", size_button);
	to_save->SetFloat2("Size", size);
	to_save->SetColor("Color", { actual_color.x,actual_color.y,actual_color.z ,actual_color.w});
	to_save->SetBoolean("HasTexture", (tex != nullptr) ? true : false);

	if (tex != nullptr)
		to_save->SetString("TextureID", std::to_string(tex->GetID()));

	to_save->SetBoolean("Enabled", enabled);
}

void ComponentButton::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
	size_button = to_load->GetFloat2("SizeButton");
	size = to_load->GetFloat2("Size");
	Color c = to_load->GetColor("Color");
	actual_color = { c.r, c.g,c.b, c.a };
	enabled = to_load->GetBoolean("Enabled");

	if (to_load->GetBoolean("HasTexture")) {
		u64 ID = std::stoull(to_load->GetString("TextureID"));
		tex = (ResourceTexture*)App->resources->GetResourceWithID(ID);
		if (tex != nullptr)
			tex->IncreaseReferences();
	}



}



bool ComponentButton::DrawInspector()
{
	if (ImGui::CollapsingHeader("Button", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Normal Color", (float*)& normal_color);
		ImGui::Spacing();
		ImGui::ColorEdit4("Hover Color", (float*)& hover_color);
		ImGui::Spacing();
		ImGui::ColorEdit4("Pressed Color", (float*)& pressed_color);


		ImGui::Text("Size");

	/*	if ()
		{
			LOG("siz")
		}*/

		if (ImGui::DragFloat("X", &size.x, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
			size_button = size;
		if (ImGui::DragFloat("Y", &size.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
			size_button = size;


	}
	ImGui::Spacing();
	ImGui::Separator();


	return true;
}

void ComponentButton::BindTex()
{
	//ComponentImage* img = game_object_attached->GetComponent<ComponentImage>();


	if (tex != nullptr && tex->id > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex->id);

	}
}



