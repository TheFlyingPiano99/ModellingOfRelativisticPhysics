#include "TextEntry.h"
#include "HUD.h"

 TextEntry::~TextEntry() {
	 if (tytle != nullptr) {
		 delete tytle;
	 }
	 if (entered != nullptr) {
		 delete entered;
	 }
}

bool TextEntry::type(char c)
{
	if (c == 8) {
		entry = entry.substr(0, entry.size() - 1);
	}
	else if (c == 13) {		// Enter
		handler(entry.c_str());
		return true;		// Submited entry
	}
	else if (c == 27) {		// Escape
		return true;		// Canceled entry
	}
	else {
		entry += c;
	}
	entered->changeText(entry.c_str());
	return false;			// Not finished
}

void TextEntry::deleteCaptions()
{
	if (tytle != nullptr) {
		delete tytle;
		tytle = NULL;
	}
	if (entered != nullptr) {
		delete entered;
		entered = NULL;
	}
}
