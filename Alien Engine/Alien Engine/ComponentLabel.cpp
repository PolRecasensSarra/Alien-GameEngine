#include "ComponentLabel.h"

ComponentLabel::ComponentLabel(GameObject* attached) :Component(attached)
{
	type = ComponentType::LABEL;
}

ComponentLabel::~ComponentLabel()
{
}
