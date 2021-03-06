#pragma once

#include "Resource_.h"
#include <vector>
#include "MathGeoLib/include/Math/float3.h"

class ResourceMesh;
class GameObject;


class ResourcePrefab : public Resource {

public:

	ResourcePrefab();
	virtual ~ResourcePrefab();

	// meta data
	bool CreateMetaData(GameObject* object, const char* folder = nullptr);
	bool ReadBaseInfo(const char* assets_file_path);
	bool DeleteMetaData();
	void Save(GameObject* prefab_root);
	void OpenPrefabScene();

	// create GameObjects
	void ConvertToGameObjects(GameObject* parent, int list_num = -1, float3 pos = { 0,0,0 }, bool set_selected = true);

};