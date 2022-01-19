#pragma once

#include "IControlEvent.h"

/*
* enum ControlEventEnum {
	togglePause,
	toggleObserver,
	toggleViewMode,
	toggleIntersectionMode,
	toggleSelection,
	toggleDopplerEffect,
	toggleShading,
	moveCameraForward,
	moveCameraBackward,
	moveCameraUp,
	moveCameraLeft,
	moveCameraRight,
	zoomIn,
	zoomOut,
	windTime,
	rewindTime,
	toggleLorentz,
	toggleTransformToProperFrame,
	save,
	load
};

*/

/*
* Used to handle user input.
* Stores key shortcut.
*/
class ControlEvent : public IControlEvent
{

public:
	virtual void performAction(const float dt) = 0;

	char getKeyShortcut() {
		return keyShortcut;
	}
};


class TogglePauseEvent : public ControlEvent {
public:
	TogglePauseEvent()
	{
		keyShortcut = ' ';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;

};

class ToggleObserverEvent : public ControlEvent {
public:
	ToggleObserverEvent()
	{
		keyShortcut = 'c';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleViewModeEvent : public ControlEvent {
public:
	ToggleViewModeEvent()
	{
		keyShortcut = 'v';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleIntersectionModeEvent : public ControlEvent {
public:
	ToggleIntersectionModeEvent()
	{
		keyShortcut = 'i';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleSelectionEvent : public ControlEvent {
public:
	ToggleSelectionEvent()
	{
		keyShortcut = 'o';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleDopplerEffectEvent : public ControlEvent {
public:
	ToggleDopplerEffectEvent()
	{
		keyShortcut = 'd';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleShadingEvent : public ControlEvent {
public:
	ToggleShadingEvent() {
		keyShortcut = 's';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class MoveCameraForwardEvent : public ControlEvent {
public:
	MoveCameraForwardEvent()
	{
		keyShortcut = '[';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class MoveCameraBackwardEvent : public ControlEvent {
public:
	MoveCameraBackwardEvent() {
		keyShortcut = '\'';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};


class MoveCameraUpEvent : public ControlEvent {
public:
	MoveCameraUpEvent()
	{
		keyShortcut = '/';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class MoveCameraDownEvent : public ControlEvent {
public:
	MoveCameraDownEvent()
	{
		keyShortcut = '.';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};


class MoveCameraLeftEvent : public ControlEvent {
public:
	MoveCameraLeftEvent()
	{
		keyShortcut = ';';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class MoveCameraRightEvent : public ControlEvent {
public:
	MoveCameraRightEvent()
	{
		keyShortcut = '\\';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ZoomInEvent : public ControlEvent {
public:
	ZoomInEvent() {
		keyShortcut = '=';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ZoomOutEvent : public ControlEvent {
public:
	ZoomOutEvent() {
		keyShortcut = '-';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class WindTimeEvent : public ControlEvent {
public:
	WindTimeEvent() {
		keyShortcut = 'f';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class RewindTimeEvent : public ControlEvent {
public:
	RewindTimeEvent() {
		keyShortcut = 'r';
	}
	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleLorentzEvent : public ControlEvent {
public:
	ToggleLorentzEvent() {
		keyShortcut = 'l';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleTransformToProperFrameEvent : public ControlEvent {
public:
	ToggleTransformToProperFrameEvent()
	{
		keyShortcut = 't';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class SaveEvent : public ControlEvent {
public:
	SaveEvent()
	{
		keyShortcut = '\x13';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class LoadEvent : public ControlEvent {
public:
	LoadEvent()
	{
		keyShortcut = '\xf';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleEditorEvent : public ControlEvent {
public:
	ToggleEditorEvent()
	{
		keyShortcut = 'e';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ClearSceneEvent : public ControlEvent {
public:
	ClearSceneEvent()
	{
		keyShortcut = 0;
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class DeleteSelectedEvent: public ControlEvent {
public:
	DeleteSelectedEvent()
	{
		keyShortcut = (char)127;
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleSimultaneBoostEvent : public ControlEvent {
public:
	ToggleSimultaneBoostEvent()
	{
		keyShortcut = 'b';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleHUDEvent : public ControlEvent {
public:
	ToggleHUDEvent()
	{
		keyShortcut = 'h';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleDisplayIntersectableEvent : public ControlEvent {
public:
	ToggleDisplayIntersectableEvent()
	{
		keyShortcut = 'p';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};

class ToggleImGUIEvent : public ControlEvent {
public:
	ToggleImGUIEvent()
	{
		keyShortcut = 'g';
	}

	// Inherited via ControlEvent
	virtual void performAction(const float dt) override;
};
