#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleObjects.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "ReturnZ.h"

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
	name = "Importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	LOG("Initing Devil");

	return true;
}

bool ModuleImporter::Start()
{
	LoadTextureFile("Assets/Textures/Checkers.png");
	//LoadModelFile("Assets/Models/BakerHouse.fbx");

	return true;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	
	return true;
}

bool ModuleImporter::LoadModelFile(const char* path)
{
	bool ret = true;

	LOG("Loading %s", path);

	// if this file has been already imported just load the .alienModel
	Resource* model = nullptr;
	if (!App->resources->Exists(path, &model)) {
		
		const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);
		
		if (scene != nullptr) {
			InitScene(path, scene);
			LOG("Succesfully loaded %s", path);
		}
		else {
			ret = false;
			LOG("Error loading model %s", path);
			LOG("Error type: %s", aiGetErrorString());
		}
		aiReleaseImport(scene);
		App->resources->AddNewFileNode(path, true);
		
	}
	else {
		App->resources->CreateNewModelInstanceOf(model->GetLibraryPath());
	}
	
	return ret;
}

void ModuleImporter::InitScene(const char* path, const aiScene* scene)
{
	model = new ResourceModel();
	model->name = App->file_system->GetBaseFileName(path);
	model->path = std::string(path);
	
	
	TakeMainTransforms(scene, scene->mRootNode);

	// start recursive function to all nodes
	LoadSceneNode(scene->mRootNode, scene, nullptr, 1);

	// create the meta data files like .alien
	if (model->CreateMetaData()) {
		App->resources->AddResource(model);
		model->ConvertToGameObjects();
		ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, App->objects->GetRoot(false)->children.back());
	}

	model = nullptr;
}

void ModuleImporter::TakeMainTransforms(const aiScene* scene, const aiNode* node)
{
	// TODO save meta
	std::string node_name = node->mName.C_Str();
	if (node_name.find("_$AssimpFbx$_") != std::string::npos || node_name.find("RootNode") != std::string::npos) {
		aiVector3D translation, scaling;
		aiQuaternion rotation;
		// local pos, rot & scale
		aiMatrix4x4 mat;
		mat.Decompose(scaling, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		float3 scale(scaling.x, scaling.y, scaling.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

		float4x4 matf;
		matf = matf.FromTRS(pos, rot, scale);
		model->main_trans = model->main_trans * matf;
		for (uint i = 0; i < node->mNumChildren; ++i) {
			TakeMainTransforms(scene, node->mChildren[i]);
		}
	}
}


void ModuleImporter::LoadSceneNode(const aiNode* node, const aiScene* scene, ResourceMesh* parent, uint family_number)
{
	LOG("Loading node with name %s", node->mName.C_Str());
	ResourceMesh* next_parent = nullptr;
	
	std::string node_name = node->mName.C_Str();
	if (node_name.find("_$AssimpFbx$_") == std::string::npos && node_name.find("RootNode") == std::string::npos) {
		if (node->mNumMeshes == 1) {
			const aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
			next_parent = LoadNodeMesh(scene, node, mesh, parent);
			next_parent->family_number = family_number;
			App->resources->AddResource(next_parent);
			model->meshes_attached.push_back(next_parent);
		}
		else if (node->mNumMeshes > 1) {
			ResourceMesh* parent_node = new ResourceMesh();
			parent_node->family_number = family_number;
			App->resources->AddResource(parent_node);
			model->meshes_attached.push_back(parent_node);
			parent_node->SetName(node->mName.C_Str());
			if (parent != nullptr)
				parent_node->parent_name = parent->name;

			for (uint i = 0; i < node->mNumMeshes; ++i) {
				const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				next_parent = LoadNodeMesh(scene, node, mesh, parent_node);
				next_parent->name += std::to_string(i);
				next_parent->family_number = family_number + 1;
				App->resources->AddResource(next_parent);
				model->meshes_attached.push_back(next_parent);
			}
			next_parent = parent_node;
		}
		else if (node->mNumMeshes == 0) {
			next_parent = new ResourceMesh();
			next_parent->family_number = family_number;
			App->resources->AddResource(next_parent);
			model->meshes_attached.push_back(next_parent);
			next_parent->SetName(node->mName.C_Str());
			if (parent != nullptr)
				next_parent->parent_name = parent->name;
		}

		// look for cameras
		if (scene->HasCameras()) {
			for (uint i = 0; i < scene->mNumCameras; ++i) {
				if (App->StringCmp(next_parent->name.data(), scene->mCameras[i]->mName.C_Str())) {
					aiCamera* cam = scene->mCameras[i];
					next_parent->has_camera = true;
					next_parent->near_plane = cam->mClipPlaneNear;
					next_parent->far_plane = cam->mClipPlaneFar;
					next_parent->horitzontal_fov = cam->mHorizontalFOV;
					next_parent->look_at.x = cam->mLookAt.x;
					next_parent->look_at.y = cam->mLookAt.y;
					next_parent->look_at.z = cam->mLookAt.z;
					next_parent->cam_up.x = cam->mUp.x;
					next_parent->cam_up.y = cam->mUp.y;
					next_parent->cam_up.z = cam->mUp.z;
					next_parent->cam_pos.x = cam->mPosition.x;
					next_parent->cam_pos.y = cam->mPosition.y;
					next_parent->cam_pos.z = cam->mPosition.z;
					if (node->mNumMeshes == 0) {
						// get local transformations
						aiVector3D translation, scaling;
						aiQuaternion rotation;
						// local pos, rot & scale
						aiMatrix4x4 mat;
						cam->GetCameraMatrix(mat);
						mat.Decompose(scaling, rotation, translation);

						// set the scale in value of 1 but keeping the dimensions
						//float max_ = max(scaling.x, scaling.y);
						//max_ = max(max_, scaling.z);

						float3 pos(translation.x, translation.y, translation.z);
						//float3 scale(scaling.x / max_, scaling.y / max_, scaling.z / max_);
						float3 scale(scaling.x, scaling.y, scaling.z);
						Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

						next_parent->pos = next_parent->cam_pos;
						next_parent->scale = scale;
						next_parent->rot = rot;
					}
				}
			}
		}
	}
	for (uint i = 0; i < node->mNumChildren; ++i) {
		LOG("Loading children of node %s", node->mName.C_Str());
		uint fam_num = 1;
		if (next_parent != nullptr)
			fam_num = next_parent->family_number + 1;
		LoadSceneNode(node->mChildren[i], scene, next_parent, fam_num);
	}
}

ResourceMesh* ModuleImporter::LoadNodeMesh(const aiScene * scene, const aiNode* node, const aiMesh* ai_mesh, ResourceMesh* parent)
{
	ResourceMesh* ret = new ResourceMesh();
	if (parent != nullptr)
		ret->parent_name = parent->name;
	
	// get vertex
	ret->vertex = new float[ai_mesh->mNumVertices * 3];
	memcpy(ret->vertex, ai_mesh->mVertices, sizeof(float) * ai_mesh->mNumVertices * 3);
	ret->num_vertex = ai_mesh->mNumVertices;

	// get index
	if (ai_mesh->HasFaces())
	{
		ret->num_index = ai_mesh->mNumFaces * 3;
		ret->index = new uint[ret->num_index]; // assume each face is a triangle
		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			if (ai_mesh->mFaces[i].mNumIndices != 3) {
				uint non[3] = { 0,0,0 };
				memcpy(&ret->index[i * 3], non, 3 * sizeof(uint));
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else {
				memcpy(&ret->index[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	// get normals
	if (ai_mesh->HasNormals())
	{
		ret->normals = new float[ai_mesh->mNumVertices * 3];
		memcpy(ret->normals, ai_mesh->mNormals, sizeof(float) * ai_mesh->mNumVertices * 3);

		ret->center_point_normal = new float[ai_mesh->mNumFaces * 3];
		ret->center_point = new float[ai_mesh->mNumFaces * 3];
		ret->num_faces = ai_mesh->mNumFaces;
		for (uint i = 0; i < ret->num_index; i+=3)
		{
			uint index1 = ret->index[i] * 3;
			uint index2 = ret->index[i + 1] * 3;
			uint index3 = ret->index[i + 2] * 3;

			float3 x0(ret->vertex[index1], ret->vertex[index1 + 1], ret->vertex[index1 + 2]);
			float3 x1(ret->vertex[index2], ret->vertex[index2 + 1], ret->vertex[index2 + 2]);
			float3 x2(ret->vertex[index3], ret->vertex[index3 + 1], ret->vertex[index3 + 2]);

			float3 v0 = x0 - x2;
			float3 v1 = x1 - x2;
			float3 n = v0.Cross(v1);

			float3 normalized = n.Normalized();

			ret->center_point[i] = (x0.x + x1.x + x2.x) / 3;
			ret->center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
			ret->center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

			ret->center_point_normal[i] = normalized.x;
			ret->center_point_normal[i + 1] = normalized.y;
			ret->center_point_normal[i + 2] = normalized.z;
		}
	}
	// get UV
	if (ai_mesh->HasTextureCoords(0)) {
		ret->uv_cords = new float[ai_mesh->mNumVertices * 3];
		memcpy(ret->uv_cords, (float*)ai_mesh->mTextureCoords[0], sizeof(float) * ai_mesh->mNumVertices * 3);
	}

	ret->InitBuffers();
	
	// set the material
	aiMaterial* ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
	aiString path;
	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	aiColor4D col;
	if (AI_SUCCESS == aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &col)) {
		ret->material_color.r = col.r;
		ret->material_color.g = col.g;
		ret->material_color.b = col.b;
		ret->material_color.a = col.a;
	}
	std::string normal_path = path.C_Str();
	App->file_system->NormalizePath(normal_path);
	ret->texture = App->resources->GetTextureByName(normal_path.data());

	// get local transformations
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	// local pos, rot & scale
	node->mTransformation.Decompose(scaling, rotation, translation);

	// set the scale in value of 1 but keeping the dimensions
	//float max_ = max(scaling.x, scaling.y);
	//max_ = max(max_, scaling.z);

	float3 pos(translation.x, translation.y, translation.z);
	//float3 scale(scaling.x / max_, scaling.y / max_, scaling.z / max_);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	ret->pos = pos;
	ret->scale = scale;
	ret->rot = rot;
	ret->name = std::string(node->mName.C_Str());

	return ret;
}

ResourceTexture* ModuleImporter::LoadTextureFile(const char* path, bool has_been_dropped, bool is_custom)
{
	ResourceTexture* texture = nullptr;

	if (!has_been_dropped && !App->file_system->Exists(path)) {
		return nullptr;
	}

	Resource* tex = nullptr;
	if (App->resources->Exists(path, &tex)) {

		std::string meta_path_in_assets = App->file_system->GetPathWithoutExtension(path) + "_meta.alien";
		u64 ID = App->resources->GetIDFromAlienPath(meta_path_in_assets.data());

		texture = (ResourceTexture*)App->resources->GetResourceWithID(ID);

		if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
			ApplyTextureToSelectedObject(texture);
		}
		LOG("This texture was already loaded");

		return texture;
	}
	else {
		texture = new ResourceTexture(path);

		texture->CreateMetaData();
		App->resources->AddNewFileNode(path, true);

		if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
			ApplyTextureToSelectedObject(texture);
		}
	}
	
	return texture;
}

ResourceTexture* ModuleImporter::LoadEngineTexture(const char* path)
{
	ResourceTexture* texture = nullptr;

	ILuint new_image_id = 0;
	ilGenImages(1, &new_image_id);
	ilBindImage(new_image_id);

	ilutRenderer(ILUT_OPENGL);

	if (ilLoadImage(path)) {
		iluFlipImage();
		texture = new ResourceTexture(path, ilutGLBindTexImage(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
		texture->is_custom = false;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);

		App->resources->AddResource(texture);

		LOG("Texture successfully loaded: %s", path);
	}
	else {
		LOG("Error while loading image in %s", path);
		LOG("Error: %s", ilGetString(ilGetError()));
	}

	ilDeleteImages(1, &new_image_id);

	return texture;
}

void ModuleImporter::LoadTextureToResource(const char* path, ResourceTexture* texture)
{
	ILuint new_image_id = 0;
	ilGenImages(1, &new_image_id);
	ilBindImage(new_image_id);

	ilutRenderer(ILUT_OPENGL);

	if (ilLoadImage(path)) {
		iluFlipImage();

		texture->id = ilutGLBindTexImage();
		texture->is_custom = true;
		texture->width = ilGetInteger(IL_IMAGE_WIDTH);
		texture->height = ilGetInteger(IL_IMAGE_HEIGHT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);

		LOG("Texture successfully loaded: %s", path);
	}
	else {
		LOG("Error while loading image in %s", path);
		LOG("Error: %s", ilGetString(ilGetError()));
	}

	ilDeleteImages(1, &new_image_id);
}

void ModuleImporter::ApplyTextureToSelectedObject(ResourceTexture* texture)
{
	GameObject* selected = App->objects->GetSelectedObject();
	if (selected != nullptr) {
		ComponentMaterial* material = (ComponentMaterial*)selected->GetComponent(ComponentType::MATERIAL);

		if (selected->HasComponent(ComponentType::MESH)) {
			if (material == nullptr) {
				material = new ComponentMaterial(selected);
				selected->AddComponent(material);
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_COMPONENT, material);
			}
			material->SetTexture(texture);
		}
		else
			LOG("Selected GameObject has no mesh");
	}
	
}

void ModuleImporter::LoadParShapesMesh(par_shapes_mesh* shape, ResourceMesh* mesh)
{
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);

	mesh->num_vertex = shape->npoints;
	mesh->num_index = shape->ntriangles * 3;

	mesh->vertex = new float[mesh->num_vertex * 3];
	mesh->index = new uint[mesh->num_index * 3];

	memcpy(mesh->vertex, shape->points, sizeof(float) * mesh->num_vertex * 3);
	memcpy(mesh->index, shape->triangles, sizeof(PAR_SHAPES_T) * mesh->num_index);
	
	if (shape->tcoords != nullptr) {
		mesh->uv_cords = new float[mesh->num_vertex * 3];
		memcpy(mesh->uv_cords, shape->tcoords, sizeof(float) * mesh->num_vertex * 3);
	}

	if (shape->normals != nullptr) {
		mesh->normals = new float[mesh->num_vertex * 3];

		memcpy(mesh->normals, shape->normals, sizeof(float) * mesh->num_vertex * 3);

		mesh->center_point_normal = new float[shape->ntriangles * 3];
		mesh->center_point = new float[shape->ntriangles * 3];
		mesh->num_faces = shape->ntriangles;
		for (uint i = 0; i < mesh->num_index; i += 3)
		{
			uint index1 = mesh->index[i] * 3;
			uint index2 = mesh->index[i + 1] * 3;
			uint index3 = mesh->index[i + 2] * 3;

			vec3 x0(mesh->vertex[index1], mesh->vertex[index1 + 1], mesh->vertex[index1 + 2]);
			vec3 x1(mesh->vertex[index2], mesh->vertex[index2 + 1], mesh->vertex[index2 + 2]);
			vec3 x2(mesh->vertex[index3], mesh->vertex[index3 + 1], mesh->vertex[index3 + 2]);

			vec3 v0 = x0 - x2;
			vec3 v1 = x1 - x2;
			vec3 n = cross(v0, v1);

			vec3 normalized = normalize(n);

			mesh->center_point[i] = (x0.x + x1.x + x2.x) / 3;
			mesh->center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
			mesh->center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

			mesh->center_point_normal[i] = normalized.x;
			mesh->center_point_normal[i + 1] = normalized.y;
			mesh->center_point_normal[i + 2] = normalized.z;
		}
	}
	mesh->InitBuffers();
}

ResourceMesh* ModuleImporter::LoadEngineModels(const char* path)
{
	ResourceMesh* r_mesh = nullptr;

	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);

	for (uint i = 0; i < scene->mNumMeshes; ++i)
	{
		r_mesh = LoadNodeMesh(scene, scene->mRootNode, scene->mMeshes[i], nullptr);
	}

	aiReleaseImport(scene);

	return r_mesh;
}

bool ModuleImporter::ReImportModel(ResourceModel* model)
{
	bool ret = true;
	
	const aiScene* scene = aiImportFile(model->GetAssetsPath(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);

	if (scene != nullptr) {
		model->name = App->file_system->GetBaseFileName(model->GetAssetsPath());
		this->model = model;
		// start recursive function to all nodes
		LoadSceneNode(scene->mRootNode, scene, nullptr, 1);

		// create the meta data files like .alien
		if (model->CreateMetaData(model->ID)) {
			App->resources->AddResource(model);
		}

		this->model = nullptr;
	}
	else {
		ret = false;
		LOG("Error loading model %s", model->GetAssetsPath());
		LOG("Error type: %s", aiGetErrorString());
	}
	aiReleaseImport(scene);

	return ret;
}



