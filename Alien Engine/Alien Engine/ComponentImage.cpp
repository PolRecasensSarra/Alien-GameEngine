#include "ComponentImage.h"
#include "GameObject.h"
#include "Application.h"
#include "ReturnZ.h"
#include "ResourceTexture.h"
#include "ComponentTransform.h"
#include "FileNode.h"
#include "imgui/imgui_internal.h"
ComponentImage::ComponentImage(GameObject* attach, float2 size):Component(attach)
{
	type = ComponentType::IMAGE;

	this->sizeIMG = size;
	this->size = size;

}

ComponentImage::~ComponentImage()
{
	if (texture != nullptr)
		texture->DecreaseReferences();
}

void ComponentImage::BindImg()
{
	if (texture != nullptr && texture->id > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		
	}
}

const ResourceTexture* ComponentImage::GetTexture() const
{
	return texture;
}

void ComponentImage::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentImage* img = (ComponentImage*)component;
		if (texture != nullptr)
			texture->DecreaseReferences();
		texture = img->texture;
		if (texture != nullptr)
			texture->IncreaseReferences();

		
		//color = material->color;
	}
}

void ComponentImage::CreatImgPlane()
{

	float width = sizeIMG.x;
	float height = sizeIMG.y;

	vertex[0] = float3(width / 2, -height / 2, 0);
	uv[0] = float2(1, 0);

	vertex[1] = float3(width / 2, height / 2, 0);
	uv[1] = float2(1, 1);

	vertex[2] = float3(-width / 2, -height / 2, 0);
	uv[2] = float2(0, 0);

	vertex[3] = float3(-width / 2, height / 2, 0);
	uv[3] = float2(0, 1);

	glGenBuffers(1, (GLuint*)& vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


//	glGenBuffers(1, (GLuint*)& texture->id);
	glBindBuffer(GL_ARRAY_BUFFER, texture->id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, uv, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)& indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentImage::UpdateImgPlane()
{

	


	vertex[0] = float3(sizeIMG.x / 2, -sizeIMG.y / 2, 0);
	vertex[1] = float3(sizeIMG.x / 2, sizeIMG.y / 2, 0);
	vertex[2] = float3(-sizeIMG.x / 2, -sizeIMG.y / 2, 0);
	vertex[3] = float3(-sizeIMG.x / 2, sizeIMG.y / 2, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId); //aixo potser no o si 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentImage::PostUpdate()
{

	Draw();
}

void ComponentImage::Draw()
{
	UpdateImgPlane();
	
//-------------------------------------------------------------------------------------------
	if (texture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);

		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, texture->id);

		glBindBuffer(GL_ARRAY_BUFFER, vertexId);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, texture->id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	}

	//------------------------------------------------------------

	glBegin(GL_QUADS);
	glLineWidth(8.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = game_object_attached->GetComponent<ComponentTransform>()->GetGlobalPosition();


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

bool ComponentImage::DrawInspector()
{
	ImVec2 min_space = ImGui::GetWindowContentRegionMin();
	ImVec2 max_space = ImGui::GetWindowContentRegionMax();

	min_space.x += ImGui::GetWindowPos().x;
	min_space.y += ImGui::GetWindowPos().y;
	max_space.x += ImGui::GetWindowPos().x;
	max_space.y += ImGui::GetWindowPos().y;

	// drop project files


	if (ImGui::CollapsingHeader("Image", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		

		ImGui::Text("Size");

		if (ImGui::DragFloat("X", &size.x, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
			sizeIMG = size;
		if (ImGui::DragFloat("Y", &size.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static))
			sizeIMG = size;

		if (texture != nullptr)
			ImGui::Image((ImTextureID)texture->id, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		else
			if (ImGui::Button("targergraphic", ImVec2(100, 100)))
			{
			}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::GetDragDropPayload();
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


						texture = texture_dropped; //id incorrect
						/*if (App->objects->GetSelectedObject()->HasComponent(ComponentType::MATERIAL))
							ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, App->objects->GetSelectedObject()->GetComponent(ComponentType::MATERIAL));
						App->importer->ApplyTextureToSelectedObject(texture_dropped);*/
						texture->IncreaseReferences();
						if (!createIMG)
						{
							CreatImgPlane();
							createIMG = true;
						}
					}

				}

			}
			ImGui::GetDragDropPayload();

			ImGui::EndDragDropTarget();

		}


	}
	ImGui::Spacing();
	ImGui::Separator();


	return true;
}