#include "TextEntry.h"
#include "HUD.h"

inline TextEntry::~TextEntry() {
	((HUD*)owner)->removeCaption(tytle);
	((HUD*)owner)->removeCaption(entered);
}

void TextEntry::type(char c)
{
	if ((int)c == 8) {
		entry.substr(0, entry.size() - 1);
	}
	else {
		entry += c;
	}
	entered->changeText(entry.c_str());
}
