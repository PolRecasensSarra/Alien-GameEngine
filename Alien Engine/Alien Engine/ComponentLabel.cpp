#include "ComponentLabel.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ReturnZ.h"
#include "ResourceFont.h"
#include "Application.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"
#include "ResourceTexture.h"



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

	InitRender();
}

ComponentLabel::~ComponentLabel()
{
	if (text_img != nullptr)
		text_img->DecreaseReferences();
}



void ComponentLabel::PostUpdate()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);

	//	if(!new_word)
	Draw();

	if (text_img != nullptr)
		UpdateLabel();

	glBindVertexArray(vertexId);
	glActiveTexture(GL_TEXTURE0);


	glDisable(GL_ALPHA_TEST);
}

void ComponentLabel::Draw()
{
	UpdateTextPlane();

	if (labelWord.empty())
	{
	/*	glBegin(GL_QUADS);
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
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);*/
	}
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


	//vertex[0] = float3(pos.x, pos.y, pos.z);
	//vertex[1] = float3(pos.x + (size.x * size_mult.x), pos.y, pos.z);
	//vertex[2] = float3(pos.x + (size.x * size_mult.x), pos.y + (size.y * size_mult.y), pos.z);
	//vertex[3] = float3(pos.x, pos.y + (size.y * size_mult.y), pos.z);

	//glBindBuffer(GL_ARRAY_BUFFER, vertexId); 
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentLabel::UpdateLabel()
{
	/*if (new_word)
	{
		if (!labelWord.empty())
		{
			for (LabelLetter* letter : labelWord)
			{

				if (letter->rect != nullptr)
				{
					RELEASE(letter->rect);
				}

				RELEASE(letter);
			}

			labelWord.clear();

		}*/

		if (text_img && !text_img->fontData.charactersMap.empty() && strcmp(finalText.c_str(),"EditText")!=0 && new_word == true)
		{
			int cursor_x = size_text.x;

			float size_norm = size.x / (float)text_img->fontData.fontSize;

			uint row_counter = 0;
			for (std::string::const_iterator c = finalText.begin(); c != finalText.end(); ++c)
			{

				if ((int)(*c) >= 32 && (int)(*c) < 128)//ASCII TABLE
				{


					//LabelLetter* l = new LabelLetter();

					Character character;
					character = text_img->fontData.charactersMap.find(*c)->second;
					float xpos = pos.x + character.bearing.x * size_norm;
					float ypos = pos.y - (character.size.y - character.bearing.y) * size_norm;

					float w = character.size.x * size_norm;
					float h = character.size.y * size_norm;

					GLfloat vertices[6][4] = {
						{ xpos,     ypos + h,   0.0, 0.0 },
						{ xpos,     ypos,       0.0, 1.0 },
						{ xpos + w, ypos,       1.0, 1.0 },

						{ xpos,     ypos + h,   0.0, 0.0 },
						{ xpos + w, ypos,       1.0, 1.0 },
						{ xpos + w, ypos + h,   1.0, 0.0 }
								};

					glBindTexture(GL_TEXTURE_2D, character.textureID);
					glBindBuffer(GL_ARRAY_BUFFER, vertexId);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// Render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);

					xpos += (character.advance >> 6)* size_norm;

					//		//
					//		memcpy(&l->letter, c._Ptr, sizeof(char));

					//		l->texture_id = character.textureID;

					//		tex = new ResourceTexture();
					//		tex->id = l->texture_id;
					//		//char_totex = new ResourceTexture()
					//		int x = cursor_x + character.bearing.x * size_norm;
					//		tex->width = x;

					//		//						Normalize pos with all heights	 //	Check Y-ofset for letters that write below origin "p" //	 Control lines enters
					//		int y = size_text.y + ((text_img->fontData.maxCharHeight - character.size.y) + ((character.size.y) - character.bearing.y)) * size_norm + row_counter * text_img->fontData.maxCharHeight * size_norm;

					//		tex->height = y;

					//		size = { (float)x,(float)y };
					//		size_text = size;
					//		new_word = false;
					//		return;
					//		if (x + character.size.x * size_norm > size.x)
					//		{
					//			y += text_img->fontData.maxCharHeight * size_norm;
					//			x = size.x + character.bearing.x * size_norm;
					//			cursor_x = x;
					//			row_counter++;
					//		}

					//		l->rect = new ComponentTransform(game_object_attached);
					//		l->rect->SetLocalPosition(x, y, game_object_attached->GetComponent<ComponentTransform>()->local_position.z);
					//		l->rect->Update();

					//		if (y + character.size.y * size_norm > y+ size.y)
					//		{
					//			if (labelWord.size() < 50) //max letters 
					//				labelWord.push_back(l);
					//			else
					//			{
					//				LOG("Label can't draw more than 50 letters.");
					//				RELEASE(l->rect);
					//				RELEASE(l);
					//				break;
					//			}
					//		}
					//		else
					//		{
					//			RELEASE(l->rect);
					//			RELEASE(l);
					//			break;
					//		}


					//		cursor_x += character.advance * size_norm;

					//	}

					//	else if ((int)(*c) == 10)//"\n"
					//	{
					//		row_counter++;
					//		cursor_x = size.x;
					//	}
					//}

				}
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//new_word = false;
//	}

	if (tex != nullptr)
	{
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
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

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);*/

	}
		
	

	
	/*text_img->fontData;*/
	//text_img->charactersBitmap;
}

void ComponentLabel::InitRender()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos			//Tex
		1.0f,  1.0f,	1.0f, 1.0f,
		1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,

		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f
	};

	glGenVertexArrays(1, &vertexId);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	glBindVertexArray(vertexId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ComponentLabel::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, float3 color)
{
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(255, 255, 255, 1.0f);


}


