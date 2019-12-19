#pragma once

#include "Component.h"

class ComponentCanvas;

enum InteractiveStates
{
	NO_STATE,
	ENTER,
	HOVER,
	EXIT
};

class ComponentUI :public Component 
{
public:
	ComponentUI(GameObject* attached);
	~ComponentUI();

	void Update();

	virtual void DoLogicClicked() {}
	virtual void DoLogicHovered() {}
	virtual void DoLogicPressed() {}

	void UpdateStates();
	void Move();

public:
	InteractiveStates state = NO_STATE;
	bool dragable = false;
	ComponentCanvas* incanvas = nullptr;

};