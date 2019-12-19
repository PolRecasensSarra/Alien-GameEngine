#include "ComponentTransform2D.h"

ComponentTransform2D::ComponentTransform2D(GameObject* attached):Component(attached)
{
	type = ComponentType::TRANSFORM2D;
}

ComponentTransform2D::~ComponentTransform2D()
{
}
