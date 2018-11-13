#ifndef _SHAPEGRAMMAR_H_
#define _SHAPEGRAMMAR_H_

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>
#include "Rule.h"

using namespace std;

class ShapeGrammar
{
public:
    ShapeGrammar();
    ShapeGrammar(const char* filename);
    ~ShapeGrammar();
	void add_rule(Rule r);

    void debug();

private:
    /*struct Rule{
        string LHS;
        string RHS;
        float prob;
    };*/

    vector<Rule> rules;
};

#endif