#pragma once

#include <string>

#define MAX_META_DATA_CHAR 200

typedef unsigned int uint;

enum class ResourceType {
	RESOURCE_NONE = -1,
	RESOURCE_MESH,
	RESOURCE_MODEL,
	RESOURCE_TEXTURE,

	RESOURECE_MAX
};


class Resource {

public:

	Resource();
	virtual ~Resource();

	// meta data path
	const char* const GetAssetsPath() const;
	const char* const GetLibraryPath() const;

	// name
	const char* const GetName() const;

	// type
	const ResourceType GetType() const;

	virtual void CreateMetaData() {} // when someting is dropped
	virtual bool ReadMetaData(char* path) { return true; } // when engine is being inicialized read all meta data

protected:

	ResourceType type = ResourceType::RESOURCE_NONE;

	std::string path;
	std::string meta_data_path;
	std::string name;

};