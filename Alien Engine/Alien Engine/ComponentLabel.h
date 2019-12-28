#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "ModuleImporter.h"
#include <map>
#include<string>

struct SDL_Surface;
struct Texture;
struct Font;

struct Character;
class ComponentTransform;

struct LabelLetter {
	char letter = NULL;
	uint texture_id = 0;
	//ComponentTransform* rect = nullptr;
	//SDL_Rect rect;
	ComponentTransform* rect = nullptr;
	
	//float2* rect;
};
class ComponentLabel :public Component 
{
	friend class CompZ;

public:
	ComponentLabel(GameObject* attach, float2 size = { 0.0f,0.0f }, bool is_custom = false);
	~ComponentLabel();

	void PostUpdate();
	void Draw();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void CreateTextPlane();

	void SetResourceFont(ResourceFont* r_font);

	bool DrawInspector();
	const ResourceFont* GetText()const;
	void UpdateTextPlane();

	void UpdateLabel();

	//
	bool GenerateText();

	void UpdateText(); //new this one
	void BindTex();
private:
	uint indexId = 0;
	uint vertexId = 0;
	uint index[6]{
	0,1,2,
	2,3,0
	};

	float3 vertex[4];
	float2 uv[4];
	float2 size_text;

	uint textureid=0;

	bool update_tetx = false;
	bool first_time = false;

	SDL_Surface* s_font = nullptr;
public:
	Font* text = nullptr;

	std::string text_str;
	char* input_text = nullptr;
	bool update_text = false;

	int max_input = 20;
	int text_size = 12;
	uint id_font = 0;



	//

	ResourceFont* text_img = nullptr;

	bool CreateText = false;
	bool new_word = false;

	std::vector<LabelLetter*> labelWord;

	std::string finalText ="EditText";


	ResourceTexture* tex = nullptr;

	void InitRender();


	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, float3 color);
public:
	bool is_custom = false;

	float3 pos;



};