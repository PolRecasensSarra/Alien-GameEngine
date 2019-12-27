
#ifndef __RESOURCE_FONT_H__
#define __RESOURCE_FONT_H__

#include "Resource_.h"
#include "MathGeoLib\include\Math\float2.h"

#include <ft2build.h>
#include <vector>
#include <map>
#include FT_FREETYPE_H

struct Character
{
	uint textureID;
	math::float2 size; //w,h
	math::float2 bearing;
	uint advance;
};
struct ResourceFontData
{
	uint fontSize;
	uint maxCharHeight;
	std::map<char, Character> charactersMap;
	std::vector<uint8_t*> fontBuffer;
};
class ResourceFont : public Resource {

public:
	ResourceFont() { type = ResourceType::RESOURCE_FONT; }
	ResourceFont(const char* path, const uint& id, const uint& width, const uint& height);
	ResourceFont(const char* path) { this->path = std::string(path); type = ResourceType::RESOURCE_FONT;}

	virtual ~ResourceFont();
	bool CreateMetaData(const u64& force_id = 0);
	bool LoadMemory();
	void FreeMemory();
	bool ReadBaseInfo(const char* assets_path);
	bool DeleteMetaData();


	//testing

	bool ImportFont(const char* path, uint ttff_size);

	static uint LoadTextureCharacter(uint width, uint height, uchar* buffer);


public:

	std::map<char, Character> charactersBitmap;
	uint maxHeight = 0;

	ResourceFontData fontData;


	bool is_custom = true;

	uint height = 0;
	uint width = 0;
	uint id = 0;

};


#endif