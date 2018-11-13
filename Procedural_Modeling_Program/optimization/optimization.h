#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <glm\glm.hpp>
#include "..\modelling\Shape.h"
#include "..\visualisation\render.h"
//#include "Graph/SimpleGraph.h"

using namespace std;

//structure of 2d vector
struct Vector2 {
	double x;
	double y;
};

namespace optimization
{

	//main class for creating boundaries
	class Bound
	{
		Vector2 Point1, Point2;
	public:
		Bound(const Vector2& p1, const Vector2& p2);

		const Vector2& get_point1() const {return Point1;}
		const Vector2& get_point2() const {return Point2;}

		Vector2 closest_point(const Vector2& p);
		double distance_to(const Vector2& a);

	};

	class optimize
	{
	private:
		Shape* room;
		double l, w;//length and width of the floor
		vector<Bound> boundaries;
		vector<Shape*> furnitures;
		vector<Vector2> centers;
		vector<double> theta;
		vector<double> prior_dis;
		vector<double> prior_angle;
		vector<double> diagonal;
		vector<double> prior_pair;
		// visualisation::render render_opt;//will be used to render room every kth iteration
	public:
		optimize(Shape* room);
		~optimize();
		void debug();
		void getBounds();
		void getFurnitures();
		void getPriors();
		void getDiagonals();
		double Prior_distance_cost(vector<Vector2> new_centers);
		double Prior_orientation_cost(vector<double> new_theta);
		double Prior_pairwise_distance_cost();
		double Accessibility_cost(vector<Vector2> new_centers);
		double Visibility_cost();//TODO use visibility cost at least for the door
		double Total_cost(double Ca, double Cd_pr, double Ctheta_pr, double Cd_pair);
		double calculate_cost_functions(vector<Vector2> new_centers, vector<double> new_theta);
		double acceptance_probability(double current_c, double new_c, double temp);
		vector<Vector2> generate_random_example(vector<Vector2> centers, int rand);
		vector<double> generate_random_theta(vector<double> new_theta);//TODO trick theta so it nevers rotates more than 180degrees
		void randomInitialPosition();
		void debugSimAn(int iteration, double current_c, double new_c, double temp, double access, double rand);
		//void drawGraph(vector<double> xs, vector<double> ys);
		void Simulated_annealing();

	};

}

#endif