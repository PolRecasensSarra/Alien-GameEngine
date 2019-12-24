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

	App->resources->AddResource(this);

	return ret;
}

bool ResourceFont::DeleteMetaData()
{
	return false;
}

bool ResourceFont::ImportFont(const char* path, uint ttff_size, u64 ID)
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

		for (uint c = 32; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				LOG("Failed to load Glyph from Freetype");
				continue;
			}

			GLuint texture = LoadTextureCharacter(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

			// Now store character for later use
			Character character = {
				texture,
				math::float2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				math::float2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x / 64
			};
			charactersBitmap.insert(std::pair<char, Character>(c, character));
			if (face->glyph->bitmap.rows > maxHeight)
				maxHeight = face->glyph->bitmap.rows;


			////Save buffer for next loads
			//uint bufferSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
			//uint8_t* characterBuffer = new uint8_t[bufferSize];
			//uint bytes = sizeof(uint8_t) * bufferSize;
			//memcpy(characterBuffer, (uint8_t*)face->glyph->bitmap.buffer, bytes);

			//fontData.fontBuffer.push_back(characterBuffer); //this font data may be saved not as a stack of memory sino pel seu valor en total en un json (meta)
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(face);

		/*ResourceData data;
		data.file = file;
		App->fs->GetFileName(file, data.name);
		data.name += std::to_string(size);

		uuid = uuid != 0 ? uuid : App->GenerateRandomNumber();
		data.exportedFile = DIR_LIBRARY_FONT + std::string("/") + data.name + "_" + std::to_string(uuid) + ".fnt";
		*/
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	return texture;
}