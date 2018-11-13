#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <iomanip>
#include "model.h"
#include <queue>
#include <string>



model::model() {
    root = nullptr;
	//create_Test_Data();
	//generateShape();
}

model::~model() {
}

std::vector<float> model::get_vector_float(std::string a)
{
	std::vector<float> nr;
	std::string help = "";
	int size = a.size();
	for (int i = 0; i < size; i++)
	{
		if (a[i] == ',')
		{
			nr.push_back(std::stof(help));
			help="";
		}
		else
		{
			//std::cout << "smth" << std::endl;
			help += a[i];
			//i++;
			//std::cout << "---"<<help <<"---"<< std::endl;
			//nr.push_back(std::stof(help));
			//help="";
		}
	}
	nr.push_back(std::stof(help));
	help = "";
	return nr;
}

std::string get_string_from_parenthesis(std::string a)
{
	std::string help = "";
	int size = a.size();
	for (int i = 1; i < size; i++)
	{
		if (a[i] != ')')
		{
			help+=a[i];
		}
		else
		{
			break;
		}
	}

	return help;
}


bool get_shape_by_name(std::string name,std::vector<Shape*>shapes)
{
	int size = shapes.size();
	for (int i = 0; i < size; i++)
	{
		if (shapes.at(i)->getName() == name)
		{
			return true;
		}
	}
	return false;
}

Shape* get_shape(std::string name, std::vector<Shape*>shapes)
{
	int size = shapes.size();
	for (int i = 0; i < size; i++)
	{
		if (shapes.at(i)->getName() == name)
		{
			return shapes.at(i);
		}
	}
	return nullptr;
}

Rule* get_rule(std::string name, std::vector<Rule*>rules)
{
	int size = rules.size();
	for (int i = 0; i < size; i++)
	{
		if (rules.at(i)->get_predecessor() == name)
		{
			return rules.at(i);
		}
	}
	return nullptr;
}

bool check_successors_exist(std::vector<Shape*>shapes, std::vector<Rule*>children)
{
	int children_size = children.size();
	int shape_size = shapes.size();
	bool is_true = false;
	for (int i = 0; i < children_size; i++)
	{
		Rule *a = children.at(i);
		for (int j = 0; j < shape_size; j++)
		{
			//std::cout << a->get_predecessor() << "-------" << shapes.at(j)->getName() << std::endl;
			if (a->get_predecessor() == shapes.at(j)->getName())
			{
				is_true = true;
			}
		}
		if (!is_true) return false;
	}
	return is_true;
}

bool check_active_rules_queue(std::vector<Rule*>r)
{
	int size = r.size();
	for (int i = 0; i < size; i++)
	{
		if (r.at(i)->is_active())
		{
			return true;
		}
	}
	return false;
}

void model::create_shapes(std::vector<Rule*>rules)
{
	int size = rules.size();
	std::vector<Rule*> my_queue;
	for (int i = 0; i < size; i++)
	{

		if (rules.at(i)->get_obj_name() != "")
		{
			//primitive shape
			Rule *a = rules.at(i);
			auto new_shape = new Shape(a->get_predecessor(), a->get_obj_name());
			std::vector<float> pos = a->get_center();
			std::vector<float> siz = a->get_size();
			if (pos.size() == 3)
				new_shape->setPosition(pos[0], pos[1], pos[2]);
			new_shape->setScale(1, 1, 1);
			if (siz.size() == 3)
			{
				new_shape->setWidth(siz[0]);
				new_shape->setLength(siz[1]);
				new_shape->setHeight(siz[2]);
			}
			//std::cout << new_shape->getName() << std::endl;
			//if (a->get_rotatey() != 0)
			//{
			new_shape->setAngle(a->get_rotatey());
			//}
			new_shape->setModelName(a->get_obj_name());
			shapes.push_back(new_shape);
			a->set_active(0);
		}
		else
		{
			my_queue.push_back(rules.at(i));
		}
	}
	int si = shapes.size();
	for (int i = 0; i < si; i++)
	{
		//shapes.at(i)->debug();
	}
	//int index = 0;
	//we have created primitive shapes so now we check each rule in queue if all its successors are shapes that are already created
	while (check_active_rules_queue(my_queue))
	{
		size = my_queue.size();
		//if (index==0)
		//std::cout <<"Index:------------------------------------ "<< index << std::endl;
		//index++;
		for (int i = 0; i < size; i++)
		{
			Rule *a = my_queue.at(i);
			//std::cout << "True############################################ " << a->get_predecessor() << std::endl;
			if (check_successors_exist(shapes, a->get_successors()) && a->is_active())
			{
				//std::cout << "True############################################ " << a->get_predecessor() << std::endl;
				a->set_active(0);
				//check if shape already created and just add successors
				Shape*head = get_shape(a->get_predecessor(), shapes);
				if (head != nullptr)
				{
					//std::cout << "True " << a->get_predecessor() << std::endl;
					//find each successors shape and then add them
					int children_size = a->get_successors().size();
					for (int k = 0; k < children_size; k++)
					{
						Rule *b = a->get_successors().at(k);
						Shape *a = get_shape(b->get_predecessor(), shapes);
						//a->debug();
						head->addChild(a);
					}
				}

				//else create new complex shape
				else
				{
					//std::cout<<"-------------------------" <<a.get_predecessor() <<std::endl;
					//std::cout << "False----------------------------------------------- " << a->get_predecessor() << std::endl;
					auto new_shape = new Shape(a->get_predecessor(), a->get_obj_name());
					std::vector<float> pos = a->get_center();
					std::vector<float> siz = a->get_size();
					new_shape->setModelName(a->get_obj_name());
					if (pos.size() > 0) new_shape->setPosition(pos[0], pos[1], pos[2]);
					new_shape->setScale(1, 1, 1);
					if (siz.size() > 0)
					{
						new_shape->setWidth(siz[0]);
						new_shape->setLength(siz[1]);
						new_shape->setHeight(siz[2]);
					}
					//find each successors shape and then create the complex shape
					int children_size = a->get_successors().size();
					for (int k = 0; k < children_size; k++)
					{
						Rule *b = a->get_successors().at(k);
						Shape *c = get_shape(b->get_predecessor(), shapes);
						new_shape->addChild(c);
					}
					shapes.push_back(new_shape);
				}
			}
		}
	}

	//fix pairwise relationships
    size = rules.size();
	for (int i = 0; i < size; i++)
	{
		Rule*a = rules.at(i);
		int pairwise_size = a->get_pairwise_distances().size();
		for (int k = 0; k < pairwise_size; k++)
		{
			if (a->get_pairwise_distances().at(k)!=a->get_predecessor())
			{
				//std::cout << a->get_pairwise_distances().at(k) << std::endl;
				Rule*pair_rule = get_rule(a->get_pairwise_distances().at(k),rules);
				Shape*pair_shape = get_shape(pair_rule->get_predecessor(), shapes);
				Shape* sh = get_shape(a->get_predecessor(), shapes);
				sh->addPairWiseDistance(pair_shape);
			}

		}
	}


	//find root
	size = shapes.size();
	for (int i = 0; i < size; i++)
	{
		//std::cout << startSymbol << std::endl;
		if (shapes.at(i)->getName() == startSymbol)
		{
			root = shapes.at(i);
		}
	}

}

void model::display_tree()
{
	std::queue<Shape*> my_queue;
	my_queue.push(root);
	std::cout << "Root: " << root->getName() << std::endl;
	while (!my_queue.empty())
	{
		Shape*a = my_queue.front();
		//std::cout << a->getName() << std::endl;
		int children_size = a->getChildren().size();
		//std::cout << children_size << std::endl;
		for (int i = 0; i < children_size; i++)
		{
			std::cout << a->getName() << "->" << a->getChildren().at(i)->getName() << std::endl;
			my_queue.push(a->getChildren().at(i));
		}
		my_queue.pop();
	}
}

void model::get_terminals(std::string line)
{	
	int size = line.size();
	std::string help;
	std::istringstream iss(line);
	while (std::getline(iss, help, ',')) {
		Terminals.push_back(help);
	}
}

void model::get_subdiv_part(Rule *r,std::string line, int id)
{
	std::string other_part=line.substr(id);
	//std::cout << other_part << std::endl;
	std::string help = "";
	int size = line.size();
	std::vector<std::string> params;
	int other_parenthesis_idx = other_part.find(")");
	std::string axis="";
	for (int i = id+8; i < size; i++)
	{
		//std::cout << line[i] << std::endl;
		if (line[i] == ')')
		{
			params.push_back(help);
			//std::cout << help << std::endl;
			help = "";
			break;
		}
		if (line[i] != '\"' && axis == "")
		{
			help += line[i];
		}
		else if (line[i] == '\"' && axis == "")
		{
			axis = help;
			help = "";
			i++;
			i++;
		}
		if (line[i] != ',' && axis!="")
		{
			help += line[i];
		}
		else if (line[i] == ',' && axis != "")
		{
			params.push_back(help);
			//std::cout << help << std::endl;
			help = "";
		}
	}
	//std::cout << axis << std::endl;

	other_parenthesis_idx = other_part.find("{");
	//int other_parenthesis_idx2 = other_part.find("}");
	for (int i = other_parenthesis_idx+1; i < size; i++)
	{
		if (line[i] == '}')
		{
			Rule *r1=new Rule();
			r1->set_predecessor(help);
			//std::cout << help << std::endl;
			r->add_successors(r1);
			help = "";
			break;
		}
		if (line[i] != '|')
		{
			help += line[i];
		}
		else
		{
			Rule *r1 = new Rule();
			r1->set_predecessor(help);
			//std::cout << help << std::endl;
			r->add_successors(r1);
			help = "";
		}
	}
}

void model::get_comp_part(Rule *r, std::string line, int id)
{
	std::string other_part = line.substr(id);
	int size = other_part.size();
	std::string help = "";
	std::string new_successor = "";
	int idx2 = 0;
	//std::cout << other_part << std::endl;
	int idx1 = other_part.find("{");
	for (int i = idx1+1; i < size;i++)
	{
		if (other_part[i] != '}')
		{
			help += other_part[i];
		}
		else
		{
			new_successor = help;
			help = "";
			idx2 = i;
			break;
		}
	}
	Rule *r1=new Rule();
	//std::cout << new_successor << std::endl;
	r1->set_predecessor(new_successor);
	r->add_successors(r1);
	//check to see if there is a new Comp
	std::string other_part2 = other_part.substr(idx2+1);
	//std::cout << other_part2 << std::endl;
	int compidx = other_part2.find("Comp(");
	while (compidx != -1)
	{
		idx1 = other_part2.find("{");
		for (int i = idx1 + 1; i < size; i++)
		{
			if (other_part2[i] != '}')
			{
				help += other_part2[i];
			}
			else
			{
				new_successor = help;
				help = "";
				idx2 = i;
				break;
			}
		}
		//std::cout << new_successor << std::endl;
		Rule *r2=new Rule();
		r2->set_predecessor(new_successor);
		r->add_successors(r2);
		other_part2 = other_part2.substr(idx2);
		compidx = other_part2.find("Comp");
	}
}

//this function open and reads grammar file
//we also create the derivation tree and the shapes for the renderer

void model::readFile(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;

    if (file.is_open()) {
		//first line is start symbol
		std::getline(file, line);
		int equalIdx = line.find("=");
		startSymbol = line.substr(equalIdx + 1);
		//second line is terminal symbols
		//std::getline(file, line);
		//equalIdx = line.find("=");
		//get_terminals(line.substr(equalIdx + 1));
		while (std::getline(file, line))
		{

			int arrowIdx = line.find("->");
			Rule *r = new Rule();
			if (arrowIdx != -1)
			{
				//first part has rule id and rule symbol
				std::string first_part = line.substr(0, arrowIdx);
				int idx= first_part.find(":");
				std::string id = first_part.substr(0, idx);
				std::string symbol= first_part.substr(idx+1);
				//std::cout << id << std::endl;
				//std::cout << symbol << std::endl;
				r->set_id(std::stoi(id));
				r->set_predecessor(symbol);
				//second part has center coordinates, COMP function SUBDIV function and size measures
				std::string second_part = line.substr(arrowIdx + 2);
				
				//find center
				int centeridx = second_part.find("C(");
				if (centeridx != -1)
				{
					std::string other_part = second_part.substr(centeridx);
					int other_parenthesis_idx = other_part.find_first_of(")");
					std::string center_string = other_part.substr(centeridx + 2, other_parenthesis_idx - 2);
					//std::cout << center_string << std::endl;
					r->set_center(get_vector_float(center_string));
				}
				
				//find size
				int sizeidx = second_part.find("S(");
				if (sizeidx != -1)
				{
					std::string other_part = second_part.substr(sizeidx);
					int other = other_part.find(")");
					std::string size_string = other_part.substr(2, other - 2);
					r->set_size(get_vector_float(size_string));

				}
				
				//find rotatex
				int rotatexidx = second_part.find("Rx(");
				if (rotatexidx != -1)
				{
					std::string other_part = second_part.substr(rotatexidx);
					int other = other_part.find(")");
					
					r->set_rotatex(std::stof(other_part.substr(3, other - 3)));
				}

				//find rotatey
				int rotateyidx = second_part.find("Ry(");
				if (rotateyidx != -1)
				{
					std::string other_part = second_part.substr(rotateyidx);
					int other = other_part.find(")");
					//std::cout << "Rotatey" << std::endl;
					r->set_rotatey(std::stof(other_part.substr(3, other - 3)));
				}

				//find rotatez
				int rotatezidx = second_part.find("Rz(");
				if (rotatezidx != -1)
				{
					std::string other_part = second_part.substr(rotatezidx);
					int other = other_part.find(")");
					r->set_rotatez(std::stof(other_part.substr(3, other - 3)));
				}

				//find obj name
				int objidx = second_part.find("I('");
				if (objidx != -1)
				{
					std::string other_part = second_part.substr(objidx);
					int other_parenthesis_idx = other_part.find("')");
					r->set_obj_name(other_part.substr(3, other_parenthesis_idx - 3));
				}

				//find Comp
				int compidx = second_part.find("Comp(");
				//std::cout<< compidx <<std::endl;
				if (compidx != -1)
				{
					get_comp_part(r, second_part, compidx);
				}

				//find Subdiv
				int subdidx = second_part.find("Subdiv(");
				if (subdidx != -1)
				{
					get_subdiv_part(r,second_part, subdidx);
				}

				//find pair
				int pairidx = second_part.find("pair(");
				if (pairidx != -1)
				{
					std::string other_part = second_part.substr(pairidx);
					//int other_parenthesis_idx = other_part.find("')");
					//std::string middle = other_part.substr(5, other_parenthesis_idx-2 );
					std::string sh = get_string_from_parenthesis(second_part.substr(pairidx + 4));
					//int commaindex = middle.find(",");
					//std::string newrule = middle.substr(0, commaindex );
					//std::string dis = middle.substr(commaindex + 1, 2);
					//std::cout << dis << std::endl;
					r->add_pairwise_distance(sh);
					//std::cout << middle << std::endl;
				}
			}
			else
			{
				//TODO PRIORITY PART
				
			}
			if(r->get_id()!=0)
				//std::cout << r->get_predecessor() << std::endl;
				rules.push_back(r);
			
		}
    }
	else
	{
		std::cout << "Error reading file!" << std::endl;
	}
   
	//createTree(rules);


	//displayTree();

    //Display(root,1);

    file.close();
}

void model::create_Test_Data()
{
	//Shape room(400, 400, 200, 800, 800, 400);
	//room.setSymbol("cube");
	//room.setName("Room");

	//Shape wall1(400, 10, 200, 800, 20, 400);
	//wall1.setSymbol("cube");
	//wall1.setName("Wall1");

	//Shape wall2(10, 400, 200, 20, 800, 400);
	//wall2.setSymbol("cube");
	//wall2.setName("Wall2");

	//Shape wall3(400, 800, 200, 800, 20, 400);
	//wall3.setSymbol("cube");
	//wall3.setName("Wall3");

	//Shape wall4(800, 800, 200, 20, 800, 400);
	//wall4.setSymbol("cube");
	//wall4.setName("Wall4");

	//Shape ceiling(400, 400, 400, 800, 800, 20);
	//ceiling.setSymbol("cube");
	//ceiling.setName("ceiling");

	//Shape floor(400,400,0, 800, 800, 20);
	//floor.setSymbol("cube");
	//floor.setName("floor");
	//

	//Shape window(400, 400, 10, 20, 200, 100);
	//window.setSymbol("cube");
	//window.setName("window");

	//Shape door(125, 10, 125, 150, 20, 250);
	//door.setSymbol("cube");
	//door.setName("door");

	//Shape bed(200, 650, 20, 120, 60, 40);
	//bed.setSymbol("cube");
	//bed.setName("bed");
	//

	//Shape closet(10, 510, 150, 20, 200, 300);
	//closet.setSymbol("cube");
	//closet.setName("closet");
	//

	//Shape chair(600, 130, 10, 60, 60, 20);
	//chair.setSymbol("cube");
	//chair.setName("chair");

	//Shape desk(600, 30, 25, 120, 60, 50);
	//desk.setSymbol("cube");
	//desk.setName("desk");
	////Shape *desk_ptr = &desk;
	////Shape *chair_ptr = &chair;
	//chair.addPairWiseDistance(desk, 20);
	//desk.addPairWiseDistance(chair, 20);
	//


	//Shape lamp1(250, 400, 400, 100, 100, 20);
	//lamp1.setSymbol("cube");
	//lamp1.setName("lamp1");
	//

	//Shape lamp2(450, 400, 400, 100, 100, 20);
	//lamp2.setSymbol("cube");
	//lamp2.setName("lamp2");

	//room.setParent(nullptr);
	//room.addChild(&wall1);
	//room.addChild(&wall2);
	//room.addChild(&wall3);
	//room.addChild(&wall4);
	//shapes.push_back(room);
	//shapes.push_back(wall1);
	//shapes.push_back(wall2);
	//shapes.push_back(wall3);
	//shapes.push_back(wall4);
	//shapes.push_back(ceiling);
	//shapes.push_back(floor);
	//shapes.push_back(window);
	//shapes.push_back(door);
	//shapes.push_back(bed);
	//shapes.push_back(closet);
	//shapes.push_back(desk);
	//shapes.push_back(chair);
	//shapes.push_back(lamp1);
	//shapes.push_back(lamp2);
}

std::vector<Shape*> model::getShapes()
{
	return shapes;
}

std::vector<Rule*> model::getRules()
{
	return rules;
}

void model::run_model(std::string filename)
{
	readFile(filename);
	int size = rules.size();
	for (int i = 0; i < size; i++)
	{
		//rules.at(i)->debug();
	}
	//std::cout << "Finished reading file" << std::endl;
	create_shapes(rules);
	size = shapes.size();
	for (int i = 0; i < size; i++)
	{
		//shapes.at(i)->debug();
	}
	//display_tree();
}



