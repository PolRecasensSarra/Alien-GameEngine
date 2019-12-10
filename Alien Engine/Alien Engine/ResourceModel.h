#pragma once

#include "Resource_.h"
#include <vector>
#include "MathGeoLib/include/Math/float4x4.h"

class ResourceMesh;

class ResourceModel : public Resource {

	friend class ModuleImporter;

public:

	ResourceModel();
	virtual ~ResourceModel();

	// meta data
	bool CreateMetaData(const u64& force_id = 0);
	bool ReadBaseInfo(const char* assets_file_path);
	bool LoadMemory();
	bool DeleteMetaData();

	// create GameObjects
	void ConvertToGameObjects();

private:

	// sort
	static bool SortByFamilyNumber(const ResourceMesh* mesh1, const ResourceMesh* mesh2);

public:

	std::vector<ResourceMesh*> meshes_attached;

private:

	float4x4 main_trans = float4x4::identity;
};