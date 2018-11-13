// // GLFW - glad - stb_image
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include "visualisation/stb_image.h"

// // GLM Mathemtics
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// GL includes
// #include "visualisation/shader.h"
// #include "visualisation/camera.h"
// #include "visualisation/model.h"
#include <iostream>
#include <typeinfo>
#include "shape.h"
#include "optimization/optimization.h"

shape* testexample(){

	double width = 15.0;//for floor, ceiling, front and back wall
	double length = 12.0;//floor, ceiling, left anf rigth wall
	double height = 10.0;//only for walls
	auto door = new shape("door", "door");
	door->setTexture("resources/door/door.jpg");
	auto window = new shape("window", "window");
	window->setTexture("resources/window/window.jpg");
	auto fwall = new shape("fwall", "wall");
	fwall->setLength(height);
	fwall->setWidth(width);
	fwall->setTexture("resources/wall/wall.jpg");
	fwall->children.push_back(door);
	auto bwall = new shape("bwall", "wall");
	bwall->setLength(height);
	bwall->setWidth(length);
	bwall->setTexture("resources/wall/wall.jpg");
	bwall->children.push_back(window);
	auto lwall = new shape("lwall", "wall");
	lwall->setLength(height);
	lwall->setWidth(length);
	lwall->setTexture("resources/wall/wall.jpg");
	auto rwall = new shape("rwall", "wall");
	rwall->setLength(height);
	rwall->setWidth(width);
	rwall->setTexture("resources/wall/wall.jpg");
	auto ceiling = new shape("ceiling", "ceiling");
	ceiling->setLength(length);
	ceiling->setWidth(width);
	ceiling->setTexture("resources/ceiling.png");
	auto floor = new shape("floor", "floor");
	floor->setLength(length);
	floor->setWidth(width);
	floor->setTexture("resources/floor/floor.jpg");
	auto box1 = new shape("box1", "furniture");
	box1->setPosition(5.0f, -4.0f, 4.0f);
	box1->setScale(2.0f, 2.0f, 2.0f);
	box1->setAngle(0);
	box1->setModelName("resources/cube_1/cube.obj");
	auto box2 = new shape("box2", "furniture");
	box2->setPosition(2.0f, -4.0f, -3.0f);
	box2->setScale(3.0f, 2.0f, 4.0f);
	box2->setAngle(0);
	box2->setModelName("resources/cube_2/cube.obj");
	box2->setPair(box1);
	auto box3 = new shape("box3", "furniture");
	box3->setPosition(-4.0f, -4.0f, 0.0f);
	box3->setScale(1.0f, 2.0f, 1.0f);
	box3->setAngle(0);
	box3->setModelName("resources/cube_3/cube.obj");

	floor->children.push_back(box1);
	floor->children.push_back(box2);
	floor->children.push_back(box3);

	auto room = new shape();
	room->children.push_back(fwall);
	room->children.push_back(bwall);
	room->children.push_back(lwall);
	room->children.push_back(rwall);
	room->children.push_back(ceiling);
	room->children.push_back(floor);

	return room;
}

int main()
{

	auto room = new shape();
	room = testexample();

	optimization::optimize optimizer(room);

	// visualisation::render test(room);

}