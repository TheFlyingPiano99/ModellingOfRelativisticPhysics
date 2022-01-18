#pragma once

class IControlEvent {
protected:
	char keyShortcut;

public:

	char getKeyShortcut() {
		return keyShortcut;
	}

	bool isPressed(const char key) {
		return key == keyShortcut;
	}

	virtual void performAction (const float dt) = 0;
};
