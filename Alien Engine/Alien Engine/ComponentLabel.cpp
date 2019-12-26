#include "ComponentLabel.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ReturnZ.h"
#include "ResourceFont.h"
#include "Application.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"

ComponentLabel::ComponentLabel(GameObject* attach, float2 size, bool is_custom) :Component(attach)
{
	type = ComponentType::LABEL;
	this->size = size;
	this->size_text = size;
	if (size.x == 0)
	{
		this->size.x = 20;
		this->size.y = 10;
	}
	this->is_custom = is_custom;

	if (!game_object_attached->HasComponent(ComponentType::TRANSFORM))
	{
		game_object_attached->AddComponent(new ComponentTransform(game_object_attached, { 0.0f,0.0f,0.0f }, { 0,0,0,0 }, { 1,1,1 }));
	}

}

ComponentLabel::~ComponentLabel()
{
}



void ComponentLabel::PostUpdate()
{
	Draw();
}

void ComponentLabel::Draw()
{
	UpdateTextPlane();

	glBegin(GL_QUADS);
	glLineWidth(8.0f);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();

	vertex[0] = float3(pos.x, pos.y, pos.z);
	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);

	glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);
	glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);
	glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
	glVertex3f(vertex[3].x, vertex[3].y, vertex[3].z);


	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
}

void ComponentLabel::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetFloat2("SizeButton", size_text);
	to_save->SetFloat2("Size", size);

	to_save->SetBoolean("Enabled", enabled);
	to_save->SetBoolean("isCustom", is_custom);
}

void ComponentLabel::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
	size_text = to_load->GetFloat2("SizeButton");
	size = to_load->GetFloat2("Size");
	enabled = to_load->GetBoolean("Enabled");
	is_custom = to_load->GetBoolean("isCutsom");
}

void ComponentLabel::CreateTextPlane()
{
	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	//pos += margin;
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();

	vertex[0] = float3(pos.x, pos.y, pos.z);
	//uv[0] = float2(0, 1);

	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	//uv[1] = float2(1, 1);

	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	//uv[2] = float2(1, 0);

	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);
	//uv[3] = float2(0, 0);

	glGenBuffers(1, (GLuint*)& vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	////	glGenBuffers(1, (GLuint*)& texture->id);
	//glBindBuffer(GL_ARRAY_BUFFER, texture->id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)& indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentLabel::SetResourceFont(ResourceFont* r_font)
{
	text_img = r_font;
}

bool ComponentLabel::DrawInspector()
{
	ImVec2 min_space = ImGui::GetWindowContentRegionMin();
	ImVec2 max_space = ImGui::GetWindowContentRegionMax();

	min_space.x += ImGui::GetWindowPos().x;
	min_space.y += ImGui::GetWindowPos().y;
	max_space.x += ImGui::GetWindowPos().x;
	max_space.y += ImGui::GetWindowPos().y;

	// drop project files


	if (ImGui::CollapsingHeader("Label", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{


		ImGui::Text("Size");

		if (ImGui::DragFloat("X", &size.x, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
		{
		//	ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			size_text = size;
		}
		if (ImGui::DragFloat("Y", &size.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
		{
		//	ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			size_text = size;
		}

		if (text_img != nullptr)
			ImGui::Image((ImTextureID)text_img->id, ImVec2(100, 100));
		else
			if (ImGui::Button("Target Graphic", ImVec2(100, 100)))
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

					ResourceFont* font_dropped = (ResourceFont*)App->resources->GetResourceWithID(ID);

					if (font_dropped != nullptr) {

						//ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
						text_img = font_dropped; 
						
						font_dropped->IncreaseReferences();
						if (!CreateText)
						{
							CreateTextPlane();
							CreateText = true;
						}
					}
					CreateText = false;
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

const ResourceFont* ComponentLabel::GetText()const
{
	return text_img;
}

void ComponentLabel::UpdateTextPlane()
{
	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();


	vertex[0] = float3(pos.x, pos.y, pos.z);
	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
