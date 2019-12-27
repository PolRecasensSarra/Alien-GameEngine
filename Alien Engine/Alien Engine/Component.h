#pragma once

#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

typedef unsigned int uint;
class JSONArraypack;
typedef unsigned long long u64;
enum class ComponentType {
	TRANSFORM = 0,
	MESH,
	MATERIAL,
	LIGHT,
	CAMERA,
	IMAGE,
	BUTTON,
	LABEL,
	CHECKBOX,
	INPUTBOX,
	TRANSFORM2D,
	CANVAS,
	UI,

	UNKNOWN
};

enum class InteractiveStates
{
	NO_STATE,
	ENTER,
	HOVER,
	EXIT
};

class Component {

public:

	Component(GameObject* attach);
	virtual ~Component();

	virtual void OnEnable() {}
	virtual void OnDisable() {}

	bool IsEnabled();
	void SetEnable(bool enable);
	virtual bool DrawInspector() { return true; }

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	virtual void Reset() {}
	virtual void SetComponent(Component* component) {}

	virtual void SaveComponent(JSONArraypack* to_save) {}
	virtual void LoadComponent(JSONArraypack* to_load) {}

	virtual bool Fade() { return false; }

	void ResetIDs();

	const ComponentType& GetType() const;
	float2 GetSize();


public:

	u64 ID = 0;
	GameObject* game_object_attached = nullptr;
	bool not_destroy = true;

protected:

	void RightClickMenu(const char* collapsing_header_name);

public:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;
	float2 size;
	float max_height = 401.0f;
	

};
