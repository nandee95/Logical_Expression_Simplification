#pragma once

#include <map>
#include "group.hpp"

using namespace std;

class Column
{
public:
	map<uint32_t, Group> groups; // Takes care of ordering aswell
	const void addToGroup(size_t group,Node node)
	{
		this->groups[group].push_back(node);
	}

	const vector<string> print()
	{
		vector<string> lines;
		for (auto it = groups.begin(); it != groups.end();it++)
		{
			for (auto& n : it->second)
			{
				lines.push_back(n.print());
			}
			if(it!= prev(groups.end()) && !it->second.empty())
			lines.push_back("-");
		}
		return lines;
	}

};