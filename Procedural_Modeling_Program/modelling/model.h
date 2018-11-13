

#include <string>
#include <vector>
#include <map>
#include "Shape.h"
#include "Rule.h"

#define numkids 6

struct node
{
    Rule value;
    node* parent;
	node *kids[numkids];
};

class model {

private:
	std::vector<Shape*> shapes;
	std::string startSymbol;
	std::vector<std::string> Terminals;
	std::vector<std::string> nonTerminals;
	std::vector<Rule*> rules;

public:
    model();
    ~model();
    Shape* root;

	//function for creating test data
	void create_Test_Data();

	//function to get all terminals from terminals=line
	void get_terminals(std::string line);
	//function to get all params and  successors from subdiv part
	void get_subdiv_part(Rule *r,std::string line, int id);
	//function to get all successors from comp part
	void get_comp_part(Rule *r, std::string line, int id);
	//most important function which parses file and creates derivation tree
    void readFile(std::string fileName);
	//function for creating a tree using the vector<Rules>
	void createTree(std::vector<Rule*>rules);

	std::vector<float> get_vector_float(std::string a);
	//void displayTree();
	//void display_tree_form(std::vector<Shape>rules);
	void create_shapes(std::vector<Rule*>rules);
	void display_tree();

	std::vector<Shape*> getShapes();
	std::vector<Rule*> getRules();
	void run_model(std::string filename);
};

