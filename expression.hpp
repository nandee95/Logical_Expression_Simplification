#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <iostream>
#include "string.hpp"
#include "column.hpp"

using namespace std;

struct SimpResult
{
	vector<vector<string>> lines;
	string table;
	string expression;
	string expression_hazard_free;
};


class Expression
{
protected:
	string expression;
	vector<char> variables;

public:
	vector<uint32_t> minterms;

	Expression(string expression);

	const uint16_t variableCount() const;

	const string listVariables();

	const bool execute(string exp);

	const void parseMinterms();

	uint8_t countOnes(uint8_t minterm);

	const SimpResult simplify();

	const void detectVariables();

	static const bool validate(string& exp);
};