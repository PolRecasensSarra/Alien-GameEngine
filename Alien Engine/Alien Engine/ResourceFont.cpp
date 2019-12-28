#include "ResourceFont.h"
#include "ModuleResources.h"
#include "Application.h"
#include "glew/include/glew.h"

#include <assert.h>
#pragma comment(lib, "Freetype/libx86/freetype.lib")

ResourceFont::ResourceFont(const char* path, const uint& id, const uint& width, const uint& height)
{
	this->path = std::string(path);
	this->id = id;
	this->width = width;
	this->height = height;
	name = App->file_system->GetBaseFileName(path);

	type = ResourceType::RESOURCE_FONT;
}

ResourceFont::~ResourceFont()
{
}

bool ResourceFont::CreateMetaData(const u64& force_id)
{
	bool ret = false;


	if (force_id == 0)
		ID = App->resources->GetRandomID();

	else
		ID = force_id;

	std::string alien_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien").data();

	JSON_Value* alien_value = json_value_init_object();
	JSON_Object* alien_object = json_value_get_object(alien_value);
	json_serialize_to_file_pretty(alien_value, alien_path.data());

	if (alien_value != nullptr && alien_object != nullptr) {
		JSONfilepack* alien = new JSONfilepack(alien_path, alien_object, alien_value);
		alien->StartSave();
		alien->SetString("Meta.ID", std::to_string(ID));
		alien->FinishSave();
		delete alien;
	}
	meta_data_path = std::string(LIBRARY_FONTS_FOLDER + std::to_string(ID) + ".ttf");
	std::string ext;
	App->file_system->SplitFilePath(path.data(), nullptr, nullptr, &ext);


	if (App->StringCmp(ext.data(), "ttf")) {
		App->file_system->Copy(path.data(), meta_data_path.data());

		ImportFont(meta_data_path.c_str(), 48);

	}

	//ImportFont()

	App->resources->AddResource(this);

	return false;
}

bool ResourceFont::LoadMemory()
{
	return false;
}

void ResourceFont::FreeMemory()
{
}

bool ResourceFont::ReadBaseInfo(const char* assets_path)
{

	bool ret = true;

	this->path = assets_path;
	std::string alien_path = App->file_system->GetPathWithoutExtension(path) + "_meta.alien";

	JSON_Value* value = json_parse_file(alien_path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(alien_path, object, value);

		ID = std::stoull(meta->GetString("Meta.ID"));

		delete meta;
	}

	meta_data_path = LIBRARY_FONTS_FOLDER + std::to_string(ID) + ".ttf";

	if (!App->file_system->Exists(meta_data_path.data())) {
		return false;
	}
	ImportFont(meta_data_path.c_str(), 48);

	App->resources->AddResource(this);

	return ret;
}

bool ResourceFont::DeleteMetaData()
{
	return false;
}

bool ResourceFont::ImportFont(const char* path, uint ttff_size)
{

	

	FT_Face face;      /* handle to face object */
	if (FT_New_Face(App->ui->library, path, 0, &face))
	{
		LOG("The font file couldn't be opened, read or this format is unsupported");
	}

	else
	{
		FT_Set_Pixel_Sizes(face, 0, ttff_size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (uint c = 32; c < 128; c++) //ascii dsde 32 fins 128
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				LOG("Failed to load Glyph from Freetype");
				continue;
			}

			GLuint texture = ResourceFont::LoadTextureCharacter(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

			// Now store character for later use
			Character2 character = {
				texture,
				math::float2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				math::float2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x / 64
			};
			charactersBitmap.insert(std::pair<char, Character2>(c, character));
			if (face->glyph->bitmap.rows > maxHeight)
				maxHeight = face->glyph->bitmap.rows;


		
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		FT_Done_Face(face);

	
		fontData.charactersMap = charactersBitmap;
		fontData.fontSize = ttff_size;
		fontData.maxCharHeight = maxHeight;
	}
	return false;
}

uint ResourceFont::LoadTextureCharacter(uint width, uint height, uchar* buffer)
{
	// Generate texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);

	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}
