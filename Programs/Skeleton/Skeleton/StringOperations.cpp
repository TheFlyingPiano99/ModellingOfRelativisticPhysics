#include "StringOperations.h"

/*
* Splits the given string to multiple substrings.
*/
std::vector<std::string> split(std::string str, const char separator) {
	std::vector<std::string> words;
	bool prevWasSeparator = true;
	for each (char c in str)
	{
		if (c == separator) {
			prevWasSeparator = true;
		}
		else {
			if (prevWasSeparator) {
				words.push_back(std::string(1, c));
			}
			else {
				words.back().append(1, c);
			}
			prevWasSeparator = false;
		}
	}
	return words;
}
