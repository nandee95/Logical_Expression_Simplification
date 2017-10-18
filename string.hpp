#pragma once

#include <string>

using namespace std;

namespace str
{
	const bool contains(string& str, string what);
	const void replace_all(string& str, string what, string with);
	std::string to_roman(unsigned int value);//http://rosettacode.org/wiki/Roman_numerals/Encode#C.2B.2B
}