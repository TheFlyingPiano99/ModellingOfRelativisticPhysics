#pragma once
#include "framework.h"


class ControlEvent
{
public:
	virtual void performAction() = 0;
};


class TogglePauseEvent : ControlEvent {
public:


	// Inherited via ControlEvent
	virtual void performAction() override;

};
