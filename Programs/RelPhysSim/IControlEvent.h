#pragma once

class IControlEvent {

protected:
	bool wasPressed = false;
	char keyShortcut;
	enum TriggerType {
		triggerContinous,
		triggerOnPress,
		triggerOnRelease
	};
	TriggerType triggerType = TriggerType::triggerOnPress;

public:

	char getKeyShortcut() const;

	bool onPress();
	bool onRelease();

	virtual void performAction (const float dt) = 0;

};
