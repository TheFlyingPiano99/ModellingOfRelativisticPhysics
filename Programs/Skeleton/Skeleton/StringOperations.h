#pragma once

#include <vector>
#include <string>

/*
* Splits the given string to multiple substrings.
*/
std::vector<std::string> split(std::string str, const char separator);

std::string join(std::vector<std::string> strings, const int firstIndex);