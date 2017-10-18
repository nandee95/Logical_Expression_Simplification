#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>
 
#include "expression.hpp"
#include "string.hpp"

using namespace std;

class Application
{
public:
	Application();

	int run();
};