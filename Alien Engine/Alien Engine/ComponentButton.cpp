#include "ComponentButton.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleUI.h"
#include "PanelGame.h"
#include "ReturnZ.h"
#include "ResourceTexture.h"
#include "FileNode.h"
#include "imgui/imgui_internal.h"


ComponentButton::ComponentButton(GameObject* attach) : Component(attach)
{
	type = ComponentType::BUTTON;

}

ComponentButton::ComponentButton(GameObject* attach, float2 size, bool is_custom) :Component(attach)
{
	type = ComponentType::BUTTON;
	
	this->size = size;
	this->size_button = size;
	this->is_custom = is_custom;

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
	if (Time::IsInGameState())
		UpdateStates();

	if (function)
	{
		//call the fade function mega hardcoded
		
		function = false;
	}
	
}

void ComponentButton::PostUpdate()
{
	Draw();
}

void ComponentButton::Draw()
{
	UpdateButtonPlane();

	BindTex();
	
	if (!tex)
	{
		CheckIfDefaulTextureIsSettedAfterReturnZ();
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
		if (transform != nullptr) {

			glBegin(GL_QUADS);
			glLineWidth(8.0f);
			glColor4f(actual_color.x, actual_color.y, actual_color.z, actual_color.w);

			float3 pos = transform->GetGlobalPosition();
			float3 size_mult = transform->GetGlobalScale();

			float3 v1 = float3(pos.x, pos.y, pos.z);
			float3 v2 = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
			float3 v3 = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
			float3 v4 = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);

			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v3.x, v3.y, v3.z);
			glVertex3f(v4.x, v4.y, v4.z);

			glEnd();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		}
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

	/*LOG("%f", origin.x);
	LOG("%f", origin.y);*/


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
	to_save->SetBoolean("isCustom", is_custom);
}

void ComponentButton::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
	size_button = to_load->GetFloat2("SizeButton");
	size = to_load->GetFloat2("Size");
	Color c = to_load->GetColor("Color");
	actual_color = { c.r, c.g,c.b, c.a };
	enabled = to_load->GetBoolean("Enabled");
	is_custom = to_load->GetBoolean("isCutsom");

	if (to_load->GetBoolean("HasTexture")) {
		u64 ID = std::stoull(to_load->GetString("TextureID"));
		if (ID == 0 && is_custom)
		{
			tex = App->resources->icons.button;
			CreatButtonPlane();
		}
		else
		{
			tex = (ResourceTexture*)App->resources->GetResourceWithID(ID);
			if (tex != nullptr)
			{
				tex->IncreaseReferences();
				CreatButtonPlane();
			}
		}

		
	}



}



bool ComponentButton::DrawInspector()
{
	ImVec2 min_space = ImGui::GetWindowContentRegionMin();
	ImVec2 max_space = ImGui::GetWindowContentRegionMax();

	min_space.x += ImGui::GetWindowPos().x;
	min_space.y += ImGui::GetWindowPos().y;
	max_space.x += ImGui::GetWindowPos().x;
	max_space.y += ImGui::GetWindowPos().y;

	// drop project files


	if (ImGui::CollapsingHeader("Button", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Normal Color", (float*)& normal_color);
		actual_color = normal_color;
		ImGui::Spacing();
		ImGui::ColorEdit4("Hover Color", (float*)& hover_color);
		ImGui::Spacing();
		ImGui::ColorEdit4("Pressed Color", (float*)& pressed_color);


		ImGui::Text("Size");



		if (ImGui::DragFloat("X", &size.x, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
		{
			ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			size_button = size;
		}

		if (ImGui::DragFloat("Y", &size.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
		{
			ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			size_button = size;
		}

		if (tex != nullptr)
			ImGui::Image((ImTextureID)tex->id, ImVec2(100, 100));
		else
			if (ImGui::Button("Target Graphic", ImVec2(100,100)))
			{
			}
		
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_SourceNoDisableHover);
					//(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_SourceNoDisableHover| ImGuiDragDropFlags_AcceptBeforeDelivery);
				if (payload != nullptr && payload->IsDataType(DROP_ID_PROJECT_NODE)) {
					FileNode* node = *(FileNode * *)payload->Data;


					// drop texture
					if (node != nullptr && node->type == FileDropType::TEXTURE && App->objects->GetSelectedObject() != nullptr)
					{
						std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name);
						path += "_meta.alien";

						u64 ID = App->resources->GetIDFromAlienPath(path.data());

						ResourceTexture* texture_dropped = (ResourceTexture*)App->resources->GetResourceWithID(ID);

						if (texture_dropped != nullptr) {
							LOG("KSDHFISSBLVKSDJMV");
							ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
							tex = texture_dropped; //id incorrect
							/*if (App->objects->GetSelectedObject()->HasComponent(ComponentType::MATERIAL))
								ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, App->objects->GetSelectedObject()->GetComponent(ComponentType::MATERIAL));
							App->importer->ApplyTextureToSelectedObject(texture_dropped);*/
							tex->IncreaseReferences();
							if (!createButtonIMG)
							{
								CreatButtonPlane();
								createButtonIMG = true;
							}
						}
						createButtonIMG = false;
						ImGui::ClearDragDrop();
					}
					
				}
				

				ImGui::EndDragDropTarget();

			}
		

	}
	ImGui::Spacing();
	ImGui::Separator();


	return true;
}

void ComponentButton::CreatButtonPlane()
{
	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();

	vertex[0] = float3(pos.x, pos.y, pos.z);
	uv[0] = float2(0, 1);

	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	uv[1] = float2(1, 1);

	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	uv[2] = float2(1, 0);

	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);
	uv[3] = float2(0, 0);

	glGenBuffers(1, (GLuint*)&vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//	glGenBuffers(1, (GLuint*)& texture->id);
	glBindBuffer(GL_ARRAY_BUFFER, tex->id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentButton::UpdateButtonPlane()
{
	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();

	vertex[0] = float3(pos.x, pos.y, pos.z);
	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId); //aixo potser no o si 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentButton::BindTex()
{
	if (tex)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);

		glColor4f(actual_color.x, actual_color.y, actual_color.z, actual_color.w);

		glEnable(GL_TEXTURE_2D);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, tex->id);

		glBindBuffer(GL_ARRAY_BUFFER, vertexId);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, tex->id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	}
}

void ComponentButton::CheckIfDefaulTextureIsSettedAfterReturnZ()
{
	if (tex == nullptr && is_custom)
	{
		tex = App->resources->icons.button;
		CreatButtonPlane();
	}
}



