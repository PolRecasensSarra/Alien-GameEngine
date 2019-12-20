#include "ComponentUI.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleInput.h"

ComponentUI::ComponentUI(GameObject* attached) : Component(attached)
{
	type = ComponentType::UI;
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::Update()
{
	//TODO::// this needs really improve, because now the UI is in the world and maybe the mouse is in the coordinates we want
	//but the button is outside the view so xD
	UpdateStates();
}

void ComponentUI::UpdateStates()
{
	float3 pos = float3::zero;
	float2 size = game_object_attached->GetSizeUI();
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	if (transform != nullptr)
		pos = transform->GetGlobalPosition();


	float width = size.x;
	float height = size.y;
	float x = pos.x;
	float y = pos.y;

	float right, left, top, bottom;
	left = x;
	right = x + width;
	top = y;
	bottom = y + height;

	//TODO::// this needs to be caped with the actual panel scene
	float mouse_x = App->input->GetMouseX();
	float mouse_y = App->input->GetMouseY();

	/*LOG("%f",mouse_x);
	LOG("%f", mouse_y);*/

	
	if (mouse_x >= left && mouse_y >= top && mouse_x <= right && mouse_y <= bottom)
	{

		if (state == NO_STATE)
		{
			state = ENTER;
			//Enter();
		}
		if (state == ENTER)
		{
			state = HOVER;
		}

		if (state == HOVER)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				DoLogicPressed();
			}
			else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				DoLogicPressed();
				if (dragable)
				{
					Move(); //I don't know if this is okai so maybe we delete this
				}
			}
			else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (!dragable)
				{
					DoLogicClicked();
				}
			}
			else
			{
				DoLogicHovered();
			}
		}
	}
	else
	{
		if (state == HOVER)
		{
			state = EXIT;
			//Exit();
		}
		else
		{
			state = NO_STATE;
			//Idle();
		}
	}
	
}


void ComponentUI::Move()
{
	float mouse_motion_x = App->input->GetMouseXMotion();
	float mouse_motion_y = App->input->GetMouseYMotion();

	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	if (transform != nullptr)
	{
		transform->local_position.x-= mouse_motion_x / 20;
		transform->local_position.y -= mouse_motion_x / 10;
	}

}
