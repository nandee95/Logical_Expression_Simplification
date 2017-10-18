#include "application.hpp"

Application::Application()
{
}

int Application::run()
{
	unique_ptr<Expression> exp;
	{	//Ask for an expression from the user
		string expression;
		bool first = true;
		do
		{
			if (!first && expression != "help")
			{
				system("cls");
				cout << "Invalid expression! Try again..." << endl;
			}
			cout << "Type 'help' for the syntax." << endl;
			cout << "Input expression: ";

			cin >> expression;
			first = false;

			if (expression == "help")
			{
				system("cls");
				cout << " *** Custom syntax ***" << endl;
				cout << left << setw(30) << "Groups:" << setw(30) << "(...)" << setw(30) << "Bracelets" << endl;
				cout << setw(30) << "Variables:" << setw(30) << "A,B,C,..." << setw(30) << "Capital English letters" << endl;
				cout << setw(30) << "Negation:" << setw(30) << "/A,/B /(A+B)" << setw(30) << "Slash" << endl;
				cout << setw(30) << "OR:" << setw(30) << "A+B,A+/B" << setw(30) << "OR operator" << endl;
				cout << setw(30) << "AND:" << setw(30) << "A*B,AB" << setw(30) << "AND operator" << endl;
				cout << endl;
				cout << "Example: /(A+B)C/D+(C*/D)" << endl;
				cout << endl;
				cout << " *** Minterm syntax ***" << endl;
				cout << setw(30) << "Example: (A,B,C,D)1,4,5,7,9,12,14,18,19,26,28,30" << endl << endl;
				cout << "- The first brackets contains the variables." << endl;
				cout << "- The rightmost variable is the LSB." << endl;
				cout << "- After the brackets the minterm numbers are comma serperated." << endl;
				cout << "- The minterm numbers must be ordered correctly!" << endl;

				cout << "Press any key to return..." << endl;

				system("pause>nul");
				system("cls");
			}

		} while (!Expression::validate(expression));
		exp = make_unique<Expression>(expression);

		system("cls");
		cout << "Input expression: " << expression << endl;
	} //'regex' , 'first' , 'expression' destructs here	


	cout << exp->variableCount() << " variables detected: " << exp->listVariables() << endl;

	const vector<uint32_t>& minterms = exp->minterms;

	cout << minterms.size() << "/" << pow(2, exp->variableCount()) << " minterms found: ";

	for (auto& m : minterms)
	{
		cout << m << " ";
	}

	cout << endl;

	SimpResult r = exp->simplify();

	cout << "Result: " << r.expression << endl;

	cout << "Details:" << endl;

	uint32_t gCount = 1;

	uint32_t tallest_value = 0;

	vector<int32_t> widths;

	// Find the tallest column and widths of the columns
	for (size_t i = 0; i < r.lines.size(); i++) //Columns
	{
		if (r.lines[i].size() > tallest_value)
		{
			tallest_value = r.lines[i].size();
		}
		uint32_t widest_value = 0;
		for (auto& s : r.lines[i]) //Rows
		{
			if (s.size() > widest_value) widest_value = s.size();
		}

		widths.push_back(widest_value);
	}

	for (auto& w : widths) w = max(15, w + 3);

	cout << left;

	//Headers
	for (size_t g = 0; g < r.lines.size(); g++)
	{
		cout << setw(widths[g]) << (str::to_roman(g + 1) + ".");
	}
	cout << endl;
	for (size_t g = 0; g < r.lines.size(); g++)
	{
		string delim;
		for (int32_t i = 0; i < widths[g] - 3; i++)
			delim += '-';
		cout << setw(widths[g]) << delim;
	}
	cout << endl;

	//Content
	for (size_t i = 0; i < tallest_value; i++)
	{
		for (size_t g = 0; g < r.lines.size(); g++)
		{
			if (r.lines[g].size() <= i) // Empty place
			{
				cout << setw(widths[g]) << " ";
				continue;
			}
			if (r.lines[g][i][0] == '-') //Lines
			{
				string delim;
				for (int32_t i = 0; i < widths[g] - 3; i++)
					delim += '-';
				cout << setw(widths[g]) << delim;
			}
			else cout << setw(widths[g]) << r.lines[g][i];
		}
		cout << endl;
	}
	system("pause>nul");
	return 1;
}
