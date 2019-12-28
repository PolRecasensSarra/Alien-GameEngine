#include "ComponentLabel.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ReturnZ.h"
#include "ResourceFont.h"
#include "Application.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"
#include "ResourceTexture.h"
#include "SDL2_ttf/include/SDL_ttf.h"



ComponentLabel::ComponentLabel(GameObject* attach, float2 size, bool is_custom) :Component(attach)
{


	type = ComponentType::LABEL;
	this->size = size;
	this->size_text = size;

	
	text = App->fonts->Load("Assets/Fonts/OpenSans-Regular.ttf");
	text_str = "Label";

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

	
//	InitRender();
}

ComponentLabel::~ComponentLabel()
{
	TTF_CloseFont(text->font);
	delete text;

	if (text_img != nullptr)
		text_img->DecreaseReferences();
}



void ComponentLabel::PostUpdate()
{

	Draw();
}

void ComponentLabel::Draw()
{
	GenerateText();
	UpdateText();
	BindTex();
	
}

void ComponentLabel::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetFloat2("SizeText", size_text);
	to_save->SetFloat2("Size", size);
	to_save->SetBoolean("HasFont", (text_img != nullptr) ? true : false);

	if (text_img != nullptr)
		to_save->SetString("FontID", std::to_string(text_img->GetID()));

	to_save->SetBoolean("Enable", enabled);
}

void ComponentLabel::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
	size_text = to_load->GetFloat2("SizeText");
	size = to_load->GetFloat2("Size");
	enabled = to_load->GetBoolean("Enabled");

	if (to_load->GetBoolean("HasFont")) {

		u64 Font_ID = std::stoull(to_load->GetString("FontID"));
		text_img = (ResourceFont*)App->resources->GetResourceWithID(Font_ID);
		if (text_img != nullptr)
		{
			text_img->IncreaseReferences();
			CreateTextPlane();
		}
	}
}

void ComponentLabel::CreateTextPlane()
{
	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	//pos += margin;
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();

	vertex[0] = float3(pos.x, pos.y, pos.z);
	uv[0] = float2(0, 1);

	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	uv[1] = float2(1, 1);

	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	uv[2] = float2(1, 0);

	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);
	uv[3] = float2(0, 0);

	glGenBuffers(1, (GLuint*)& vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, (GLuint*)& textureid);
	glBindBuffer(GL_ARRAY_BUFFER, textureid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, (GLuint*)& indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentLabel::SetResourceFont(ResourceFont* r_font)
{
	text_img = r_font;
	LOG("new font set to %s", game_object_attached->GetName());
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

		//ImGui::InputText("Test", &finalText)
		static char namm[50];
		memcpy(namm, finalText.c_str(), 50);

		if (ImGui::InputText("##Set Text", namm,50))
		{
			finalText = namm;

		}
	
		if (ImGui::Button("Done"))
		{
			new_word = true;
		}
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
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_AcceptBeforeDelivery);
			//(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_SourceNoDisableHover| ImGuiDragDropFlags_AcceptBeforeDelivery);
			if (payload != nullptr && payload->IsDataType(DROP_ID_PROJECT_NODE)) {
				FileNode* node = *(FileNode * *)payload->Data;


				// drop texture
				if (node != nullptr && node->type == FileDropType::FONT && App->objects->GetSelectedObject() != nullptr)
				{
					std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name);
					path += "_meta.alien";

					u64 ID = App->resources->GetIDFromAlienPath(path.data());

					ResourceFont* font_dropped = (ResourceFont*)App->resources->GetResourceWithID(ID);

					if (font_dropped != nullptr) {

						//ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
						text_img = font_dropped; 

						text_img->IncreaseReferences();
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
	pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();


	vertex[0] = float3(pos.x, pos.y, pos.z);
	vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


bool ComponentLabel::GenerateText()
{
	if (text_str.empty()) {
		return false;
	}

	UpdateTextPlane();
}

void ComponentLabel::UpdateText()
{
	if (text == nullptr)
		return;

	if (!text->font || text_str.empty())
		return;


	update_text = true;

	int w = size.x, h = size.y;

	TTF_SizeText(text->font, text_str.c_str(), &w, &h);

	if (!first_time)
	{

		
	}

	s_font = TTF_RenderText_Blended_Wrapped(text->font, text_str.c_str(), SDL_Color{
		(Uint8)(255), (Uint8)(255),(Uint8)(255), (Uint8)(255) }, w);


	glGenTextures(1, &id_font);
	glBindTexture(GL_TEXTURE_2D, id_font);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, s_font->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(s_font);
}

void ComponentLabel::BindTex()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, id_font);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, textureid);
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



