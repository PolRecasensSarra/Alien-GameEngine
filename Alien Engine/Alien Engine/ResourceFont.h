
#ifndef __RESOURCE_FONT_H__
#define __RESOURCE_FONT_H__

#include "Resource_.h"

#include "ft2build.h"

#include FT_FREETYPE_H

#include "MathGeoLib\include\Math\float2.h"


class Resource_Font : public Resource {

public:
	Resource_Font() { type = ResourceType::RESOURCE_FONT; }
	/*Resource_Font(const char* path, const uint& id, const uint& width, const uint& height);
	Resource_Font(const char* path) { this->path = std::string(path); type = ResourceType::RESOURCE_FONT; }

	virtual ~Resource_Font();
	bool CreateMetaData(const u64& force_id = 0);
	bool LoadMemory();
	void FreeMemory();
	bool ReadBaseInfo(const char* assets_path);
	bool DeleteMetaData();
*/
public:

	bool is_custom = true;

	uint height = 0;
	uint width = 0;
	uint id = 0;

};


#endif