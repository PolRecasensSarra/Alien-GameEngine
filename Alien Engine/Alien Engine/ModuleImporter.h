#pragma once

#include "Globals.h"
#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include <vector>

struct Mesh {
	uint id_index = 0; // index in VRAM
	uint id_vertex = 0; // unique vertex in VRAM

	uint num_index = 0; // done
	std::vector<uint> index; // done

	uint num_vertex = 0; // done
	std::vector<float> vertex; // done

	uint material_index = 0; // done

	std::vector<float> normals; // done

	std::vector<float> texture_cords; // done
};

struct Textures {

};

struct FBXdata {
	std::vector<Mesh*> meshes;
	std::vector<Textures*> textures;
	const char* path = nullptr;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool LoadFBX(const char* path);

private:

	void InitScene(const aiScene* scene, const char* path);
	Mesh* InitMesh(const aiMesh* ai_mesh);
	void InitGLBuffers(Mesh* mesh);

private:

	std::vector<FBXdata*> fbx_data;

};
