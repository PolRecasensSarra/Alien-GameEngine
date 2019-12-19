#pragma once

#include "Component.h"

class ComponentCanvas;

class ComponentUI :public Component 
{
public:
	ComponentUI(GameObject* attached);
	~ComponentUI();

	virtual void DoLogicClicked() {}
	virtual void DoLogicHovered() {}
	virtual void DoLogicPressed() {}

public:
	bool dragable = false;
	ComponentCanvas* incanvas = nullptr;

};