#include "ResourceFont.h"
#include "ModuleResources.h"
#include "Application.h"
#include "glew/include/glew.h"

#include <assert.h>
#pragma comment(lib, "Freetype/libx86/freetype.lib")

//Resource_Font::Resource_Font(const char* path, const uint& id, const uint& width, const uint& height)
//{
//	this->path = std::string(path);
//	this->id = id;
//	this->width = width;
//	this->height = height;
//
//	type = ResourceType::RESOURCE_FONT;
//}
//
//Resource_Font::~Resource_Font()
//{
//}
//
//bool Resource_Font::CreateMetaData(const u64& force_id)
//{
//	bool ret = false;
//
//
//	if (force_id == 0)
//		ID = App->resources->GetRandomID();
//
//	//else
//	//	ID = force_ID
//	return false;
//}
//
//bool Resource_Font::LoadMemory()
//{
//	return false;
//}
//
//bool Resource_Font::ReadBaseInfo(const char* assets_path)
//{
//	return false;
//}
