#include "ComponentUI.h"
#include "GameObject.h"

ComponentUI::ComponentUI(GameObject* attached) : Component(attached)
{
	type = ComponentType::UI;
}

ComponentUI::~ComponentUI()
{
}
