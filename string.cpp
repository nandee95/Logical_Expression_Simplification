#include "string.hpp"

const bool str::contains(string & str, string what)
{
	return str.find(what) != std::string::npos;
}

const void str::replace_all(string & str, string what, string with)
{
	while (str.find(what) != std::string::npos) {
		str.replace(str.find(what), what.size(), with);
	}
}

//http://rosettacode.org/wiki/Roman_numerals/Encode#C.2B.2B
std::string str::to_roman(unsigned int value)
{
	struct romandata_t { unsigned int value; char const* numeral; };
	const struct romandata_t romandata[] =
	{
		{ 1000, "M" },{ 900, "CM" },
		{ 500, "D" },{ 400, "CD" },
		{ 100, "C" },{ 90, "XC" },
		{ 50, "L" },{ 40, "XL" },
		{ 10, "X" },{ 9, "IX" },
		{ 5, "V" },{ 4, "IV" },
		{ 1, "I" },
		{ 0, NULL } // end marker
	};

	std::string result;
	for (const romandata_t* current = romandata; current->value > 0; ++current)
	{
		while (value >= current->value)
		{
			result += current->numeral;
			value -= current->value;
		}
	}
	return result;
}
