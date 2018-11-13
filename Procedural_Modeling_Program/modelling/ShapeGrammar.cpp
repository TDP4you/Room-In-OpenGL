#include "ShapeGrammar.h"



ShapeGrammar::ShapeGrammar(){}
ShapeGrammar::ShapeGrammar(const char* filename)
{
    ifstream file(filename, fstream::in);
    if (!file.good())
    {
        cout << "ERROR reading file ..." << endl;
        return;
    }


	regex rule_regex("*->*");
	regex priority_regex("*PRIORITY*:$");
	regex subdiv_regex("*Subdiv(*)*");
	regex comp_regex("*Comp(*)*");
	regex repeat_regex("*Repeat(*)*");
	regex i_command_regex("*I(*)*");
	regex size_regex("*S(*)*");
	regex translation_regex("*T(*)*");
	regex rotatex_regex("*Rx(*)*");
	regex rotatey_regex("*Ry(*)*");
	regex rotatez_regex("*Rz(*)*");

    string rule;

	int priority = 0;
    while (getline(file, rule))
    {
		//cout << rule << endl;
		int separatorIdx = rule.find("->");
		//cout << separatorIdx << endl;
		if (separatorIdx == -1)
		{
			//Check if PRIORITY
			int priorityIdx = rule.find("PRIORITY");
			if (priorityIdx != -1)
			{
				vector<string> split_strings = split_string(rule, " ");
				int size = split_strings.at(1).size();
				string nr = split_strings.at(1).substr(0, size - 1);
				priority = stoi(nr);

			}
			//else its other thing
			
		}
		else
		{
			Rule r(rule, priority);
			add_rule(r);
			cout << "rule line: " << rule << endl;

			r.debug();
		}
    }
	file.close();
}
ShapeGrammar::~ShapeGrammar()
{
}

void ShapeGrammar::add_rule(Rule r)
{
	rules.push_back(r);
}

void ShapeGrammar::debug()
{
    /*for (int i = 0; i < rules.size(); ++i)
    {
		rules.at(i).debug();
    }*/
}