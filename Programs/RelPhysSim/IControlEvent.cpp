#include "IControlEvent.h"

#include <cctype>

char IControlEvent::getKeyShortcut() const {
	return std::toupper(keyShortcut);
}

bool IControlEvent::onPress()
{
	bool trigger = false;
	switch (triggerType)
	{
	case TriggerType::triggerOnPress:
		trigger = !wasPressed;
		break;
	case TriggerType::triggerContinous:
		trigger = true;
		break;
	case TriggerType::triggerOnRelease:
		trigger = false;
		break;
	default:
		break;
	}
	wasPressed = true;
	return trigger;
}

bool IControlEvent::onRelease()
{
	bool trigger = false;
	switch (triggerType)
	{
	case TriggerType::triggerOnPress:
		trigger = false;
		break;
	case TriggerType::triggerContinous:
		trigger = false;
		break;
	case TriggerType::triggerOnRelease:
		trigger = wasPressed;
		break;
	default:
		break;
	}
	wasPressed = false;
	return trigger;
}