#include "expression.hpp"

const uint16_t Expression::variableCount() const
{
	return static_cast<uint16_t>(variables.size());
}

const string Expression::listVariables()
{
	string output;
	auto& last = std::prev(variables.end());

	for (auto& v : variables)
	{
		output += v;
		output += (v == *last ? "" : " ");
	}
	return output;
}

const bool Expression::execute(string exp)
{
	//Deal with brackets
	if (str::contains(exp, "("))
	{
		size_t pos1 = -1;
		int32_t level = 0;

		for (size_t i = 0; i < exp.size(); i++)
		{

			if (exp[i] == '(')
			{
				if (level == 0) pos1 = i;
				level++;
			}
			if (exp[i] == ')')
			{
				level--;
				if (level == 0)
				{
					const size_t size = i - pos1;

					const bool negation = (pos1 > 0 && exp[pos1 - 1] == '/');
					const string inner = exp.substr(pos1 + 1, size - 1);
					exp.erase(pos1 - (negation ? 1 : 0), size + 1 + (negation ? 1 : 0));
					if (!negation) exp.insert(pos1, 1, execute(inner) ? '1' : '0');
					else exp.insert(pos1, 1, execute(inner) ? '0' : '1');
					i = pos1;
				}
			}

		}
	}

	//Deal with ANDs (neighbour variables)
	exp.erase(remove(exp.begin(), exp.end(), '*'), exp.end()); //Remove the non needed operators

	for (size_t i = 0; i< exp.size() - 1; i++)
	{
		while (isdigit(exp[i]) && isdigit(exp[i + 1]))
		{
			const bool r = exp[i] == '1' && exp[i + 1] == '1';
			exp.erase(i, 2);
			exp.insert(i, 1, r ? '1' : '0');
		}
	}

	//Deal with ORs (we look for the operators here)
	for (size_t i = 1; i< exp.size() - 1; i++)
	{
		if (exp[i] == '+')
		{
			const bool r = exp[i - 1] == '1' || exp[i + 1] == '1';
			exp.erase(i - 1, 3);
			i--;
			exp.insert(i, 1, r ? '1' : '0');
		}
	}

	return exp == "1";
}

const void Expression::parseMinterms()
{
	//Minterm form
	if (regex_match(expression, regex("^\\((?:[A-Z],)*(?:[A-Z])\\)(?:[0-9]+,)*[0-9]+$")))
	{
		string temp = expression.substr(expression.find_first_of(')') + 1, expression.size() - expression.find_first_of(')'));
		str::replace_all(temp, ",", " ");
		stringstream ss(temp);

		string buffer;
		uint32_t max = static_cast<uint32_t>(pow(2, variables.size()));
		while (ss >> buffer)
		{
			uint32_t m = atoi(buffer.c_str());

			if (m < max)
			{
				minterms.push_back(m);
			}
		}
	}

	//Custom form
	const uint32_t num = static_cast<uint32_t>(pow(2, variables.size()));
	map<char, bool> values;

	for (auto& v : this->variables)
	{
		values[v] = false;
	}


	uint32_t size = this->variables.size();

	for (uint32_t i = 0; i < num; i++)
	{
		//Update values
		for (uint32_t v = 0; v < size; v++)
		{
			values[v] = i >> v & 1;
		}

		string exp(expression);

		for (uint32_t v = 0; v < size; v++)
		{
			str::replace_all(exp, "/" + string(1, this->variables[v]), to_string(!values[v])); //Negation
			replace(exp.begin(), exp.end(), this->variables[v], to_string(values[v])[0]);
		}
		if (execute(exp))
		{
			minterms.push_back(i);
		}
	}

}

uint8_t Expression::countOnes(uint8_t minterm)
{
	uint8_t count = 0;
	for (uint8_t i = 0; i < 8; i++)
		if ((minterm >> i) & 1) count++;
	return count;
}

const SimpResult Expression::simplify()
{
	SimpResult result;

	//Special cases
	if (minterms.size() == 0)
	{
		result.expression = "0";
		return result;
	}
	else if (minterms.size() == static_cast<size_t>(pow(2, variables.size())))
	{
		result.expression = "1";
		return result;
	}

	vector<Column> cols;
	cols.resize(1);
	for (auto& m : minterms)
	{
		cols[0].addToGroup(countOnes(m), Node({ m }, {}));
	}
	auto current = 0;
	while (cols[current].groups.size() > 1)
	{
		cols.push_back(Column());
		auto next = current + 1;
		for (size_t i = 1; i < cols[current].groups.size(); i++)
		{
			const size_t p = i - 1;

			for (auto& n1 : cols[current].groups[p]) //prev
			{

				for (auto& n2 : cols[current].groups[i]) //current
				{
					if (!(n1 != n2)) continue;
					const uint32_t diff = *n2.minterms.begin() - *n1.minterms.begin();

					if (countOnes(diff) == 1 && n1.getCurrentDiff() == n2.getCurrentDiff())//rules
					{
						//Collecting data
						multiset<uint32_t> minterms;
						for (auto& m : n1.minterms)	minterms.insert(m);
						for (auto& m : n2.minterms)	minterms.insert(m);

						set<uint32_t> differences;
						differences.insert(diff);
						for (auto& d : n1.differences) differences.insert(d);
						for (auto& d : n2.differences) differences.insert(d);
						Node n(minterms, differences);

						bool contains = false;
						for (auto& cn : cols[next].groups[p])
						{
							if (n == cn)
							{
								contains = true;
								break;
							}
						}

						if (!contains && diff > n2.getCurrentDiff()) //Goes to the next column aswell
						{
							cols[next].addToGroup(p, n);
						}

						n2.checked = true;
						n1.checked = true;
					}
				}
			}
		}
		current++;
	}

	for (auto& c : cols)
	{
		result.lines.push_back(c.print());
	}

	//Determine the simplest possible expression

	for (auto& c : cols)
	{
		for (auto& g : c.groups)
		{
			for (auto& n : g.second)
			{
				if (n.checked) continue;
				result.expression += n.getMintermForm(variables) + "+";
			}
		}
	}
	if (!result.expression.empty())
		result.expression.erase(prev(result.expression.end()));


	return result;
}

const void Expression::detectVariables()
{
	for (auto& c : expression)
	{
		if (!isalpha(c)) continue; //skip operators

		bool contains = false;

		for (auto& v : variables)
		{
			if (c == v)
			{
				contains = true;
				break;
			}
		}

		if (!contains)
		{
			this->variables.push_back(c);
		}
	}
}

const bool Expression::validate(string & exp)
{
	const regex r1("^\\((?:[A-Z],)*(?:[A-Z])\\)(?:[0-9]+,)*[0-9]+$");
	if (regex_match(exp, r1))
	{
		uint32_t maxminterm = static_cast<uint32_t>(pow(2, count_if(exp.begin(), exp.end(), isalpha)));

		string temp = exp.substr(exp.find_first_of(')') + 1, exp.size() - exp.find_first_of(')'));
		str::replace_all(temp, ",", " ");
		stringstream ss(temp);

		string buffer;

		int32_t prev = 0;

		while (ss >> buffer)
		{
			int32_t m = atoi(buffer.c_str());

			if (m >= static_cast<int32_t>(maxminterm) || m < 0 || m < prev)
			{
				return false;
			}
			prev = m;
		}

		return true;
	}

	if (str::contains(exp, "/)"))	return false;


	{
		uint32_t opened = 0;

		for (auto& c : exp)
		{
			if (c == '(')
			{
				opened++;
			}
			else if (c == ')')
			{
				opened--;
				if (opened < 0) return false;
			}
		}

		if (opened != 0) return false;
	}

	string temp = exp;

	//Remove the brackets
	temp.erase(remove(temp.begin(), temp.end(), '('), temp.end());
	temp.erase(remove(temp.begin(), temp.end(), ')'), temp.end());

	//Test the regex to match the variables
	return regex_match(temp, regex("^(?:\\/?[A-Z])+(?:[+*](?:(?:\\/?[A-Z])+))*$"));
}

Expression::Expression(string expression) : expression(expression)
{
	detectVariables();
	parseMinterms();
}
