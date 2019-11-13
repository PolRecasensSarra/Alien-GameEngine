#pragma once 

#include "ModuleObjects.h"

enum class ReturnActions {
	UNKNOWKN = -1,

	DELETE_OBJECT,
	ADD_OBJECT,
};

class ReturnZ {

public:

	ReturnZ() {}
	~ReturnZ() {}

	static void AddNewAction(const ReturnActions& type, void* data);

public:

	union Actions {

		Actions() {}
		~Actions() {}

		ReturnActions type = ReturnActions::UNKNOWKN;

		struct Obj {

			bool enabled = false;
			bool is_static = false;
			u64 ID = 0;
			u64 parentID = 0;
			bool to_delete = false;
			bool parent_selected = false;
			bool selected = false;
			bool parent_enabled = false;

			std::string name = "UnNamed";

			std::vector<Obj*> children;

			struct Comp {

				uint type = 0;

				struct {
					float3 pos = float3::zero;
					float3 scale = float3::zero;
					Quat rot = Quat::identity;
					bool is_scale_negative = false;
				} transform;

			}; std::vector<Comp*> comps;

		} object;

	} action; 

private:

	ReturnZ::Actions::Obj SetDeleteObject(GameObject* obj);
	void SetAction(const ReturnActions& type, void* data);
};