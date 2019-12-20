#pragma once

#include "Component.h"

class ComponentCanvas;


class ComponentUI :public Component 
{
public:
	ComponentUI(GameObject* attached);
	~ComponentUI();

	void Update();

	virtual void DoLogicClicked() {}
	virtual void DoLogicHovered() {}
	virtual void DoLogicPressed() {}
	virtual void DoLogicExit() {}

	void UpdateStates();
	void Move();

public:
	//InteractiveStates state = NO_STATE;
	bool dragable = false;
	ComponentCanvas* incanvas = nullptr;

};