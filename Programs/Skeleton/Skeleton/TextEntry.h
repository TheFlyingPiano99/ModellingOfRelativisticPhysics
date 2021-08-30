#pragma once

#include "framework.h"
#include "Caption.h"

class TextEntry
{
	void* owner;
	vec2 pos;
	std::string entry;
	Caption* tytle = NULL;
	Caption* entered = NULL;
	void (*handler)(const char*);
public:

	TextEntry(void* _owner, vec2 _pos, const char* _tytle, void (*_handler)(const char*))
		: owner(_owner),
		pos(_pos),
		entry(),
		tytle(Caption::createNormalCameraSpaceCaption(_pos, _tytle)),
		entered(Caption::createNormalCameraSpaceCaption(_pos - vec2(0, 0.1f), "")),
		handler(_handler)
	{

	}

	~TextEntry();

	/*
	* Returns true if the entry finished.
	*/
	bool type(char c);

	std::string& getEntry() {
		return entry;
	}

	void deleteCaptions();
};

