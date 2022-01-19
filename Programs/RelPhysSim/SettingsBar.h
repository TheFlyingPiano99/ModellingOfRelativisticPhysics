#pragma once

#include <map>
#include <memory>

#include "EnumTypes.h"
#include "Caption.h"

class SettingsBar
{
	void* owner;
	vec2 pos;
	std::map<size_t, std::shared_ptr<Caption*>> captions;

public:
	SettingsBar(void* _owner, vec2 _pos) : owner(_owner), pos(_pos) {
	}

	~SettingsBar() {
		clear();
	}

	/*
	* Update displayed settings.
	*/
	void updateSettings(const RelTypes::Settings& settings);

	/*
	* Erease captions.
	*/
	void clear();

};

