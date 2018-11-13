
#include <fstream>
#include <numeric>
#include "modelling\Shape.h"
#include "modelling\model.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <regex>
#include "optimization\optimization.h"
#include "visualisation\render.h"


using namespace std;

Shape* testexample() {

	double width = 15.0;//for floor, ceiling, front and back wall
	double length = 15.0;//floor, ceiling, left anf rigth wall
	double height = 10.0;//only for walls

	auto door = new Shape("door", "door");
	door->setPosition(0.0f,-7.5,0.0f);
	door->setScale(2.0f,2.0f,2.0f);
	door->setRotationVector(0,1,0);
	door->setAngle(0);
	door->setModelName("resources/door/door.obj");

	auto window = new Shape("window1", "window");
	window->setPosition(3.2f, -1.0f, -7.4f);
	window->setScale(0.002f, 0.002f, 0.002f);
	window->setAngle(0);
	window->setRotationVector(0, 1, 0);
	window->setModelName("resources/window2/window.obj");
	auto window2 = new Shape("window2", "window");
	window2->setPosition(-3.2f, -1.0f, -7.4f);
	window2->setScale(0.002f, 0.002f, 0.002f);
	window2->setAngle(0);
	window2->setRotationVector(0, 1, 0);
	window2->setModelName("resources/window1/window.obj");
	auto fwall = new Shape("fwall", "wall");
	fwall->setLength(height);
	fwall->setWidth(width);
	fwall->setTexture("resources/wall/wall.jpg");
	fwall->children.push_back(door);
	auto bwall = new Shape("bwall", "wall");
	bwall->setLength(height);
	bwall->setWidth(length);
	bwall->setTexture("resources/wall/wall.jpg");
	bwall->children.push_back(window);
	bwall->children.push_back(window2);
	auto lwall = new Shape("lwall", "wall");
	lwall->setLength(height);
	lwall->setWidth(length);
	lwall->setTexture("resources/wall/wall.jpg");
	auto rwall = new Shape("rwall", "wall");
	rwall->setLength(height);
	rwall->setWidth(width);
	rwall->setTexture("resources/wall/wall.jpg");
	auto ceiling = new Shape("ceiling", "ceiling");
	ceiling->setLength(length);
	ceiling->setWidth(width);
	ceiling->setTexture("resources/ceiling/roof.jpg");
	auto floor = new Shape("floor", "floor");
	floor->setLength(length);
	floor->setWidth(width);
	floor->setTexture("resources/floor/floor.jpg");
	auto box1 = new Shape("box1", "furniture");
	box1->setPosition(-6.3f, -5.0f, -4.0f);
	box1->setScale(3.0f, 3.0f, 3.0f);
	box1->setAngle(-90.0f);
	box1->setModelName("resources/Bookshelf/bookshelf.obj");
	auto box2 = new Shape("box2", "furniture");
	box2->setPosition(-6.3f, -5.0f, -4.0f);
	box2->setScale(3.0f, 3.0f, 3.0f);
	box2->setAngle(-90.0f);
	box2->setModelName("resources/Book/books.obj");
	box2->addPairWiseDistance(box1);
	auto box3 = new Shape("box3", "furniture");
	box3->setPosition(-6.4f, -5.0f, 0.0f);
	box3->setScale(0.03f, 0.03f, 0.03f);
	box3->setAngle(90.0f);
	box3->setModelName("resources/Closet/closet.obj");
	auto box4 = new Shape("box3", "furniture");
	box4->setPosition(-8.05f, 1.0f, 0.0f);
	box4->setScale(2.0f, 2.0f, 2.0f);
	box4->setAngle(90.0f);
	box4->setModelName("resources/Picture/frida.obj");
	
	auto fireplace = new Shape("box3", "furniture");
	fireplace->setPosition(3.0f,-5.0f, -7.0f);
	fireplace->setScale(8.0f, 8.0f, 8.0f);
	fireplace->setAngle(0);
	fireplace->setModelName("resources/Fireplace/FP2015.obj");
	auto tv = new Shape("box3", "furniture");
	tv->setPosition(0.2f, -1.0f, -6.0f);
	tv->setScale(0.09f, 0.09f, 0.09f);
	tv->setAngle(0);
	//tv->setRotationVector(0,0,0);
	tv->setModelName("resources/HDTV/TV.obj");
	auto chair = new Shape("box3", "furniture");
	chair->setPosition(5.0f, -5.0f, -2.0f);
	chair->setScale(1.0f, 1.0f, 1.0f);
	chair->setAngle(90);
	chair->setModelName("resources/chair/office_chair.obj");
	auto sofa = new Shape("box3", "furniture");
	sofa->setPosition(0.0f, -5.0f, 3.0f);
	sofa->setScale(0.03f, 0.03f, 0.03f);
	sofa->setAngle(180);
	sofa->setModelName("resources/sofa/sofa.obj");
	auto table = new Shape("box3", "furniture");
	table->setPosition(7.0f, -5.0f, -2.0f);
	table->setScale(0.01f, 0.01f, 0.01f);
	table->setAngle(90);
	table->setModelName("resources/table/Table.obj");

	floor->children.push_back(box1);
	floor->children.push_back(box2);
	floor->children.push_back(box3);
	floor->children.push_back(box4);
	floor->children.push_back(fireplace);
	floor->children.push_back(tv);
	floor->children.push_back(chair);
	floor->children.push_back(sofa);
	floor->children.push_back(table);
	auto room = new Shape();
	room->children.push_back(fwall);
	room->children.push_back(bwall);
	room->children.push_back(lwall);
	room->children.push_back(rwall);
	room->children.push_back(ceiling);
	room->children.push_back(floor);

	return room;
}

int main(int argc, char** argv)
{
	//model model1;
	//model1.run_model("test_grammar.txt");
	auto room = new Shape();
	room = testexample();
	//room = model1.root;
	//optimization::optimize optimizer(room);
	visualisation::render render(room);
    return 0;
}