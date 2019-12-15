#pragma once

#include "Component.h"
#include <vector>
#include <string>

struct __declspec(dllexport) InspectorScriptData {
	friend class ComponentScript;
	enum DataType { 
		INT, // DONE
		INT2,
		INT3,
		FLOAT, // DONE
		FLOAT2,
		FLOAT3,
		STRING,
		BOOL,
	};

	enum ShowMode {
		INPUT_INT,
		DRAGABLE_INT,
		SLIDER_INT,
		INPUT_FLOAT,
		DRAGABLE_FLOAT,
		SLIDER_FLOAT,
		CHECKBOX,
	};

	InspectorScriptData(const std::string& variable_name, const DataType& variable_type, void* ptr, const ShowMode& mode) {
		this->variable_name = variable_name;
		this->variable_type = variable_type;
		this->ptr = ptr;
		this->show_as = mode;
	}

	std::string variable_name;
	DataType variable_type;
	ShowMode show_as;
	void* ptr = nullptr;

private:
	//ugly
	float min_slider = 0;
	float max_slider = 0;
};

class __declspec(dllexport) ComponentScript : public Component {
	friend class ReturnZ;
	friend class CompZ;
	friend class PanelInspector;
	friend class ModuleObjects;
	friend class GameObject;
public:
	ComponentScript(GameObject* attach);
	virtual ~ComponentScript();

private:

	void Reset();
	void SetComponent(Component* component);

	bool DrawInspector();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void LoadData(const char* name, bool is_alien);

	static std::string GetVariableName(const char* ptr_name);
public:
	/*--------------------INT-----------------------*/
	static void InspectorInputInt(int* ptr, const char* ptr_name);
	static void InspectorDragableInt(int* ptr, const char* ptr_name);
	static void InspectorSliderInt(int* ptr, const char* ptr_name, const int& min_value, const int& max_value);
	/*--------------------FLOAT-----------------------*/
	static void InspectorInputFloat(float* ptr, const char* ptr_name);
	static void InspectorDragableFloat(float* ptr, const char* ptr_name);
	static void InspectorSliderFloat(float* ptr, const char* ptr_name, const float& min_value, const float& max_value);
	/*--------------------BOOL-----------------------*/
	static void InspectorBool(bool* ptr, const char* ptr_name);

	static void StartEnum();
	static void EndEnum();

private:

	bool enuming = false;

	u64 resourceID = 0;
	// TODO: change to touple and next option is for personaliing ImGui inspector, like drag int text int bla bla
	// enum for types of imgui to show variables
	// change to struct, need variable name
	std::vector<InspectorScriptData> inspector_variables;
	bool need_alien = false;
	std::string data_name;
	void* data_ptr = nullptr;
};
