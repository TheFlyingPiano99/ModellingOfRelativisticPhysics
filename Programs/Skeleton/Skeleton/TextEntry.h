#pragma once

#include "framework.h"
#include "Caption.h"

class TextEntry
{
	void* owner;
	vec2 pos;
	std::string entry;
	Caption* tytle;
	Caption* entered;
public:

	TextEntry(void* _owner, vec2 _pos, const char* _tytle)
		: owner(_owner),
		pos(_pos),
		entry(),
		tytle(Caption::createNormalCameraSpaceCaption(_pos, _tytle)),
		entered(Caption::createNormalCameraSpaceCaption(_pos - vec2(0, 0.3f), ""))
	{

	}
	~TextEntry();

	void type(char c);

	std::string& getEntry() {
		return entry;
	}
};

