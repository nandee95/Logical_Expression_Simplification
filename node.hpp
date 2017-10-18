#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

class Node
{
public:
	bool checked = false;
	multiset<uint32_t> minterms;
	set<uint32_t>differences;

	Node(multiset<uint32_t> minterms, set<uint32_t> differences) : minterms(minterms), differences(differences)
	{
	}

	uint32_t getCurrentDiff()
	{
		if (differences.empty()) return 0;
		return *prev(differences.end());
	}

	const string print()
	{
		stringstream line;
		for (auto it = minterms.begin(); it != minterms.end(); it++)
		{
			line << *it;

			if (it != std::prev(minterms.end()))
				line << ",";
		}
		if (!differences.empty())
		{
			line << "(";
			for (auto it = differences.begin(); it != differences.end(); it++)
			{
				line << *it;

				if (it != std::prev(differences.end()))
					line << ",";
			}
			line << ")";
		}
		if (checked) line << "*";
		return line.str();
	}
	
	const string getMintermForm(vector<char>& variables)
	{
		stringstream result;
		for (int32_t i = variables.size() - 1; i >= 0; i--)
		{
			bool contains = false;
			for (auto& d : differences)
			{
				if (d == pow(2, i)) contains = true;
			}
			if (contains) continue;

			result << ((*minterms.begin() >> i & 1) ? "" : "/");
			result << variables[variables.size() - 1 - i];
		}
		return result.str();
	}

	bool operator == (Node& n2)
	{
		if (n2.differences.size() != this->differences.size() || n2.minterms.size() != this->minterms.size())
			return false;

		auto it1 = this->minterms.begin();
		auto it2 = n2.minterms.begin();
		while(it1!=this->minterms.end() && it2 != n2.minterms.end())
		{
			if (*it1 != *it2) return false;
			it1++; it2++;
		}

		auto it12 = this->differences.begin();
		auto it22 = n2.differences.begin();
		while (it12 != this->differences.end() && it22 != n2.differences.end())
		{
			if (*it12 != *it22) return false;
			it12++; it22++;
		}

		return true;
	}

	bool operator != (Node& n2) //Only checks the differences
	{
		if (n2.differences.size() != this->differences.size())
			return false;

		auto it12 = this->differences.begin();
		auto it22 = n2.differences.begin();
		while (it12 != this->differences.end() && it22 != n2.differences.end())
		{
			if (*it12 != *it22) return false;
			it12++; it22++;
		}

		return true;
	}
};