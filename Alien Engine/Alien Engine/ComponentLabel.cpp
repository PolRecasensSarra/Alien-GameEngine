#include "ComponentLabel.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ReturnZ.h"
#include "ResourceFont.h"
#include "Application.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"
#include "ResourceTexture.h"



ComponentLabel::ComponentLabel(GameObject* attach, std::string text, float2 size, bool is_custom) :Component(attach)
{
	type = ComponentType::LABEL;
	this->size = size;
	this->size_text = size;
	this->finalText = text;
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
	text_font = App->fonts->default_font;

	textureID = text_font->Characters.at(finalText.at(0)).TextureID;
	for (std::string::iterator it = finalText.begin(); it != finalText.end(); ++it)
	{
		//LabelLetter[it]. = text_font->Characters.at(finalText.at(0)).TextureID;

		LabelLetter l;

		l.texture_id = text_font->Characters.at(*it).TextureID;
		l.size = text_font->Characters.at(*it).Size;
		l.bearing = text_font->Characters.at(*it).Bearing;
		l.advance = text_font->Characters.at(*it).Advance;			
		CreatePPlane(l);

		word.push_back(l);


		
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
	std::vector<LabelLetter>::iterator it = word.begin();

	float2 size_counter = { 0,0 };
	while (it < word.end())
	{
		UpdateTextPlane(size_counter.x+ (*it).bearing.x,(*it));
		size_counter.x += (*it).size.x;


		BindText((*it).texture_id,(*it));
		

		if ((*it).texture_id ==0)
		{
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

		++it;
	}

	
}

void ComponentLabel::BindText(uint tex_id,LabelLetter l)
{
	
	//std::vector<LabelLetter>::iterator it = word.begin();

	//while (it != word.end())
	//{
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, tex_id);

		glBindBuffer(GL_ARRAY_BUFFER, l.vertexId);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, tex_id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l.indexId);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

void ComponentLabel::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetFloat2("SizeText", size_text);
	to_save->SetFloat2("Size", size);

	to_save->SetBoolean("Enable", enabled);
}

void ComponentLabel::LoadComponent(JSONArraypack* to_load)
{
	ID = std::stoull(to_load->GetString("ID"));
	size_text = to_load->GetFloat2("SizeText");
	size = to_load->GetFloat2("Size");
	enabled = to_load->GetBoolean("Enabled");
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

	glGenBuffers(1, (GLuint*)&vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	
	glBindBuffer(GL_ARRAY_BUFFER, textureID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

		if (ImGui::InputText("##Set Text", namm, 50))
		{
			finalText = namm;

		}

		if (ImGui::Button("Done"))
		{
			new_word = true;

			word.clear();
			for (std::string::iterator it = finalText.begin(); it != finalText.end(); ++it)
			{
				//LabelLetter[it]. = text_font->Characters.at(finalText.at(0)).TextureID;

				LabelLetter l;

				l.texture_id = text_font->Characters.at(*it).TextureID;
				l.size = text_font->Characters.at(*it).Size;
				l.bearing = text_font->Characters.at(*it).Bearing;
				l.advance = text_font->Characters.at(*it).Advance;
				CreatePPlane(l);

				word.push_back(l);



			}

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

	}
	ImGui::Spacing();
	ImGui::Separator();


	return true;
}


void ComponentLabel::UpdateTextPlane(float sizeX,LabelLetter l)
{
	pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();


	if (Time::IsInGameState())
	{
		vertex[0] = float3(pos.x+sizeX, pos.y, pos.z);
		vertex[1] = float3(pos.x + sizeX, pos.y + (size.y * size_mult.y), pos.z);
		vertex[2] = float3(pos.x + sizeX + (size.x * size_mult.x ), pos.y + (size.y * size_mult.y ), pos.z);
		vertex[3] = float3(pos.x + sizeX + (size.x * size_mult.x), pos.y, pos.z);
		uv[0] = float2(1, 1);
		uv[1] = float2(1, 0);
		uv[2] = float2(0, 0);
		uv[3] = float2(0, 1);
	}
	else
	{
		vertex[0] = float3(pos.x + sizeX, pos.y, pos.z);
		vertex[1] = float3(pos.x + sizeX + (size.x * size_mult.x), pos.y, pos.z);
		vertex[2] = float3(pos.x + sizeX + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
		vertex[3] = float3(pos.x + sizeX, pos.y + (size.y * size_mult.y), pos.z);
		uv[0] = float2(0, 1);
		uv[1] = float2(1, 1);
		uv[2] = float2(1, 0);
		uv[3] = float2(0, 0);
	}


	glBindBuffer(GL_ARRAY_BUFFER, l.vertexId); //aixo potser no o si 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, l.texture_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);
}

void ComponentLabel::CreatePPlane(LabelLetter& l)
{
	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();
	//pos += margin;
	float3 size_mult = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalScale();

	if (Time::IsInGameState())
	{
		vertex[0] = float3(pos.x, pos.y, pos.z);
		uv[0] = float2(1, 1);

		vertex[1] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);
		uv[1] = float2(1, 0);

		vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
		uv[2] = float2(0, 0);

		vertex[3] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
		uv[3] = float2(0, 1);
	}
	else
	{
		vertex[0] = float3(pos.x, pos.y, pos.z);
		uv[0] = float2(0, 1);

		vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
		uv[1] = float2(1, 1);

		vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
		uv[2] = float2(1, 0);

		vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);
		uv[3] = float2(0, 0);
	}

	glGenBuffers(1, (GLuint*)& l.vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, l.vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glBindBuffer(GL_ARRAY_BUFFER, l.texture_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)& l.indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l.indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, l.index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

