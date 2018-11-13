#include "optimization.h"

//declare global for generating andom numbers
random_device rd;
mt19937 generator(rd());

//helper functions for translating radians-degrees
double radians_to_degrees(double rad){
	return rad * 180/M_PI;
}

double degrees_to_radians(double degrees){
	return degrees * M_PI/180;
}

//set point to vector
Vector2 set_points(double x, double y) {
	Vector2 pass_value;
	pass_value.x = x;
	pass_value.y = y;

	return pass_value;
}

//helper functions for adding, subtracting and multiplying vectors
Vector2 add(const Vector2& a, const Vector2& b) {
	Vector2 c;
	c.x = a.x+b.x;
	c.y = a.y+b.y;

	return c;
}

Vector2 substract(const Vector2& a, const Vector2& b) {
	Vector2 c;
	c.x = a.x-b.x;
	c.y = a.y-b.y;

	return c;
}

Vector2 multiply(double s, const Vector2& b) {
	Vector2 c;
	c.x = s*b.x;
	c.y = s*b.y;

	return c;
}

double dot_product(const Vector2& a, const Vector2& b) {
	double c;
	c = (a.x*b.x)+(a.y*b.y);
	
	return c;
}

//helper function calculating length
double length(const Vector2& a) {
	double c;
	c = sqrt(pow(a.x,2)+pow(a.y,2));
	
	return c;
}

//Since we translate our problem to 2d we only take coordinates x, z
//we take x as x and y as z so we translate the cordinates as in 2d
Vector2 glmToVector2(glm::vec3 a) {
	Vector2 b;
	b.x = a.x;
	b.y = a.z;

	return b;
}

namespace optimization
{

	//bound class constructor
	Bound::Bound(const Vector2& p1, const Vector2& p2) {//line constructor
		Point1 = p1;
		Point2 = p2;
	}

	//calculate closest point of boundaries to another point
	Vector2 Bound::closest_point(const Vector2& p){//finds closest point in line
		Vector2 cl_point, a, b;
		a = get_point1();
		b = get_point2();
		cl_point = substract(a, multiply((dot_product(substract(a, p), substract(b, a))) / (dot_product(substract(b, a), substract(b, a))), substract(b, a)));

		return cl_point;
	}

	//calulate distance of boundary to point
	double Bound::distance_to(const Vector2& p) {//calculate distance between 2 points
		return length(substract(closest_point(p), p));
	}

	optimize::optimize(Shape* room){//constructor
		this->room = room;
		getBounds();
		getFurnitures();
		getPriors();
		getDiagonals();
		visualisation::render test(room);
		Simulated_annealing();
		// debug();
	}

	optimize::~optimize(){//destructor

	}	

	void optimize::debug(){//for debugging pruposes
		//initial test
		std::cout << "Test Optimization" << std::endl;

		//print boundaries
		for (int i = 0; i < boundaries.size(); ++i)
		{
			std::cout << "Bound " << i+1 << " points: ";
			std::cout << "(" << boundaries[i].get_point1().x << ", " << boundaries[i].get_point1().y << ") <-> ";
			std::cout << "(" << boundaries[i].get_point2().x << ", " << boundaries[i].get_point2().y << ")" << std::endl;
		}
		//print something from funriture vector
		std::cout << furnitures[0]->getType() << std::endl;
		//print a center from vector center
		std::cout << centers.size() << std::endl;
		//print prior distance
		std::cout << prior_dis[2] << std::endl;
		//print prior angle
		std::cout << prior_angle.size() << std::endl;
		//print diagonal
		std::cout << diagonal[1] << std::endl;

	}

	void optimize::getBounds(){//get for boundaries(walls) where object cannot get out
		//Get length and width of floor so we can create coordinates for oprimization
		for (int i = 0; i < room->children.size(); ++i)
		{
			if (room->children[i]->getType() == "floor")
			{
				l = room->children[i]->getLength();
				w = room->children[i]->getWidth();
			}
		}

		//create 4 boundaries
		Bound bound1 (set_points(l/2.0, -w/2.0), set_points(l/2.0, w/2.0));
		Bound bound2 (set_points(l/2.0, w/2.0), set_points(-l/2.0, w/2.0));
		Bound bound3 (set_points(-l/2.0, w/2.0), set_points(-l/2.0, -w/2.0));
		Bound bound4 (set_points(-l/2.0, -w/2.0), set_points(l/2.0, -w/2.0));

		boundaries.push_back(bound1);
		boundaries.push_back(bound2);
		boundaries.push_back(bound3);
		boundaries.push_back(bound4);
	}

	void optimize::getFurnitures(){//get furnitures from tree for faster calculation
		for (int i = 0; i < room->children.size(); ++i)
		{
			if (room->children[i]->getType() == "floor")
			{
				for (int j = 0; j < room->children[i]->children.size(); ++j)
				{
					furnitures.push_back(room->children[i]->children[j]);
					centers.push_back(glmToVector2(room->children[i]->children[j]->getPosition()));
					theta.push_back(degrees_to_radians(room->children[i]->children[j]->getAngle()));
				}
			}
		}
	}

	void optimize::getPriors(){//get priors from the positive tree example we have
		double temp;
		//we get all prior distances(distance to closest wall)
		for (int i = 0; i < centers.size(); ++i)
		{
			for (int j = 0; j < boundaries.size(); ++j)
			{
				if (j==0)
				{
					temp = boundaries[j].distance_to(centers[i]);
				} else {
					if (temp > boundaries[j].distance_to(centers[i]))
					{
						temp = boundaries[j].distance_to(centers[i]);
					}
				}
			}
			prior_dis.push_back(temp);//get prior distance
			prior_angle.push_back(furnitures[i]->getAngle());//get prior angle
		}
		for (int i = 0; i < furnitures.size(); ++i)
		{
			for (int j = 0; j < furnitures[i]->getPairwiseDistances().size(); ++j)
			{
				prior_pair.push_back(length(substract(glmToVector2(furnitures[i]->getPosition()), glmToVector2(furnitures[i]->getPairwiseDistances()[j]->getPosition()))));
			}
		}
	}

	void optimize::getDiagonals(){//get diagonals of the furnitures
		for (int i = 0; i < furnitures.size(); ++i)
		{
			double l = furnitures[i]->getScale().x;
			double w = furnitures[i]->getScale().z;
			diagonal.push_back(sqrt(pow(l,2) + pow(w,2))/2);
		}
	}

	//Prior distance cost Cd_pr
	double optimize::Prior_distance_cost(vector<Vector2> new_centers){//calculate prior distance cost
		double Cd_pr=0;
		vector<double> d;
		//calculate distance to the closest wall and save them in vectro d
		for (int i = 0; i < new_centers.size(); ++i){
			for (int j = 0; j < boundaries.size(); ++j){
				if (j == 0){
					d.push_back(boundaries[j].distance_to(new_centers[i]));
				}
				if (d[i] > boundaries[j].distance_to(new_centers[i])){
					d[i] = boundaries[j].distance_to(new_centers[i]);
				}
			}
		}
		//with vector d and prior_dis calculate the prior distance cost
		for (int i = 0; i < new_centers.size(); ++i){
			Cd_pr += sqrt(pow((d[i] - prior_dis[i]), 2));
		}
		return Cd_pr;
	}

	//Prior orientation cost Ctheta_pr
	double optimize::Prior_orientation_cost(vector<double> new_theta){//calculate orientation cost
		double Ctheta_pr=0;
		for (int i = 0; i < new_theta.size(); ++i){
			Ctheta_pr += sqrt(pow((new_theta[i] - prior_angle[i]), 2));
		}
		return Ctheta_pr;
	}

	//Prior pairwise distance cost Cd_pair
	double optimize::Prior_pairwise_distance_cost(){//calculate pair distance cost
		double Cd_pair=0;
		vector<double> d;

		//calculate distance between pairwise objects
		for (int i = 0; i < furnitures.size(); ++i){
			for (int j = 0; j < furnitures[i]->getPairwiseDistances().size(); ++j){
				furnitures[i]->getPosition();
				d.push_back(length(substract(glmToVector2(furnitures[i]->getPosition()), glmToVector2(furnitures[i]->getPairwiseDistances().at(j)->getPosition()))));
			}
		}

		for (int i = 0; i < d.size(); ++i){
			Cd_pair += sqrt(pow((d[i] - prior_pair[i]), 2));
		}
		return Cd_pair;
	}

	//penalizes as object i overlaps with object j accesible space 
	double optimize::Accessibility_cost(vector<Vector2> new_centers){//calculate accessibility cost
		double access_cost;
		double Ca=0;
		for (int i = 0; i < new_centers.size(); ++i){
			for (int j = 0; j < new_centers.size(); ++j){
				if(i!=j){//we dont want to check if object overlaps itself
					double a = length(substract(new_centers[i], new_centers[j]));
					double b = 1.2*(diagonal[i] + diagonal[j]);
					access_cost = 1 - (a / b);
					Ca += max(0.0, access_cost);
				}
			}
		}
		return Ca;
	}

	//Visibility cost Cv
	double optimize::Visibility_cost(){//TODO implement visibility cost(at least only for the door)
		double Cv = 0.0;
		return Cv;
	}

	//Total cost of the system
	double optimize::Total_cost(double Ca, double Cd_pr, double Ctheta_pr, double Cd_pair){//calculate total system cost
		double wa = 5.0;
		double wd_pr = 5.0;//not sure 
		double wtheta_pr = 10.0;
		double wd_pair = 1.0;//not sure

		return wa*Ca + wd_pr*Cd_pr + wtheta_pr*Ctheta_pr + wd_pair*Cd_pair;
	}

	double optimize::calculate_cost_functions(vector<Vector2> new_centers, vector<double> new_theta){//helper function to calculate all costs
		double Cd_pr = Prior_distance_cost(new_centers);
		double Ca = Accessibility_cost(new_centers);
		double Ctheta_pr = Prior_orientation_cost(new_theta);
		double Cd_pair = Prior_pairwise_distance_cost();//dont forget to update furntures cemters

		return Total_cost(Ca, Cd_pr, Ctheta_pr, Cd_pair);
	}

	//calculate acceptance probability of every random move
	double optimize::acceptance_probability(double current_c, double new_c, double temp){
		return min(exp(-(new_c - current_c) / temp), 1.0);
	}

	vector<Vector2> optimize::generate_random_example(vector<Vector2> new_centers, int rand){//generate random move or random swapping between 2 furnitures
		//generate random number with normal distribution
		normal_distribution<> distribution_normal(0.0, 1.0);
		//generate random number for choosing an object(candidate)
		uniform_int_distribution<> distribution_candidate(0, centers.size()-1);		

		if (rand == 0){//procced with random translation
			//declaration of dp as Vector3 and a new variable to save the new p(center of object)
			Vector2 dp, new_p;
			//decalre candidate number and choose one
			int candidate;
			candidate = distribution_candidate(generator);

			bool pass=false;

			do{
				// candidate = distribution_candidate(generator);
				//set random values to dp with normal distribution 
				dp = set_points(distribution_normal(generator), distribution_normal(generator));
				//replace new candidate to vector p
				new_p = add(new_centers[candidate], dp);
				//check if we didnt extend boundaries, if we did then generate new random number. we do this checking width and length of floor
				// if (new_p.x + (0.5*w) > 0 && new_p.x - (0.5*w) < 0 && new_p.y + (0.5*l) > 0 && new_p.y - (0.5*l) < 0)
				// {
				// 	count = 1;
				// } else {
				// 	count = 0; 
				// }
				double temp;
				//we get all prior distances(distance to closest wall)
				for (int j = 0; j < boundaries.size(); ++j)
				{
					if (j==0)
					{
						temp = boundaries[j].distance_to(new_p);
					} else {
						if (temp > boundaries[j].distance_to(new_p))
						{
							temp = boundaries[j].distance_to(new_p);
						}
					}
				}
				if (new_p.x > -(0.5*w) && new_p.x < (0.5*w) && new_p.y > -(0.5*l) && new_p.y < (0.5*l) && temp >= prior_dis[candidate])
				{
					pass = true;
				}

			}while(!pass);

			new_centers[candidate] = new_p;
			return new_centers;

		} else if (rand == 1){//procced with swapping objects
			//declare temp variables for swapping
			Vector2 temp_p;
			//declare two random candidates for swapping
			int candidate_1, candidate_2;
			candidate_1 = distribution_candidate(generator);
			candidate_2 = distribution_candidate(generator);
			//we check that candidate 1 and candidate 2 are not the same object
			while(candidate_1 == candidate_2) candidate_2 = distribution_candidate(generator);

			//swap objects centers
			temp_p = new_centers[candidate_1];
			new_centers[candidate_1] = new_centers[candidate_2];
			new_centers[candidate_2] = temp_p;

			return new_centers;
		}

	}

	vector<double> optimize::generate_random_theta(vector<double> new_theta){//generate random angle theta
		//generate random number with normal distribution
		normal_distribution<> distribution_normal(0.0, 1.0);
		//generate random number for choosing an object(candidate)
		uniform_int_distribution<> distribution_candidate(0, centers.size()-1);

		double dp;
		//decalre candidate number and choose one
		int candidate;
		int count = 0;

		candidate = distribution_candidate(generator);
		//set random value to theta with normal distribution
		dp = distribution_normal(generator);
		new_theta[candidate] = new_theta[candidate] + dp;

		while(new_theta[candidate] > 2*M_PI){
			new_theta[candidate] = new_theta[candidate] - 2*M_PI;
		}

		while(new_theta[candidate] < 0){
			new_theta[candidate] = 2*M_PI - new_theta[candidate];
		}
		
		return new_theta;
	}

	void optimize::randomInitialPosition(){//generate random starting position of furnitures
		//generate random number with normal distribution
		normal_distribution<> distribution_normal(0.0, 1.0);
		for (int i = 0; i < centers.size(); ++i)
		{
			centers[i] = set_points(distribution_normal(generator), distribution_normal(generator));
			theta[i] = distribution_normal(generator);
		}
	}

	void optimize::Simulated_annealing(){//main function for simulated annealing
		//generate vectors for centers, diagonals, prior distance
		vector<Vector2> new_centers;
		vector<double> new_theta;
		vector<double> x, fx;
		double new_c, current_c, access, rand;
		double alpha = 0.9995;
		double temp = 1000.0;
		double epsilon = 0.001;
		int iteration = 0;
		int rand_example;
		uniform_real_distribution<> distribution(0.0, 1.0);//generate random number on range [0,1)
		uniform_int_distribution<> distribution_move(0, 2);//generate random number on range [0,2], for wich random move to make

		randomInitialPosition();//give random centers and angles of objects

		for (int i = 0; i < centers.size(); ++i){//give new centers and new theta current state
			new_centers.push_back(centers[i]);
			new_theta.push_back(theta[i]);
		}

		current_c = calculate_cost_functions(centers, theta);//calculate cost function

		//give intial values to vectors for plotting temperature on a graph
		x.push_back(0);
		fx.push_back(temp);

		while(temp > epsilon){//main loop

			//generate random integer number
			rand_example = distribution_move(generator);//generate random number for rundom move

			//generate new random position
			if (rand_example == 2){
				new_theta = generate_random_theta(theta);//generate new angle theta
			} else {
				new_centers = generate_random_example(centers, rand_example);//generate new center position or swapp objects
			}

			//calculate new cost function
			new_c = calculate_cost_functions(new_centers, new_theta);

			//if the acceptance probability is bigger that uniform random number [0,1) then switch to new state
			access = acceptance_probability(current_c, new_c, temp);
			rand = distribution(generator);//random number [0,1)
			if(access > rand){//if its true then we move to the new position
				current_c = new_c;
				for (int i = 0; i < new_centers.size(); ++i){
					centers[i] = new_centers[i];
					theta[i] = new_theta[i];
					furnitures[i]->setPosition(centers[i].x, furnitures[i]->getPosition().y, centers[i].y);	
					furnitures[i]->setAngle(radians_to_degrees(theta[i]));
				}

			}

			//decrase temperature/couling down
			temp *= alpha;

			//every 1000 iterations render the current state of the room
			if (iteration % 1000 == 0)
			{
				x.push_back(iteration/1000);
				fx.push_back(temp);
			}
			if (iteration % 5000 == 0){
				debugSimAn(iteration, current_c, new_c, temp, access, rand);
				visualisation::render test(room);
			}

			//update iterations
			iteration++;
		}
		// drawGraph(x, fx);//draw temperature in descend graph
		debugSimAn(iteration, current_c, new_c, temp, access, rand);//for debugging pruposes
		visualisation::render test(room);//render final room
	}

	//for debugging pruposes
	void optimize::debugSimAn(int iteration, double current_c, double new_c, double temp, double access, double rand){
		cout << "Iteration: " << iteration << endl;
		cout << "Total Cost: " << current_c << endl;
		cout << "New total cost: " << new_c << endl;
		cout << "Temperature: "<< temp << endl;
		cout << "Acceptance probability: " << access << endl;
		cout << "Random probability: " << rand << endl;
		// cout << "Distance between 0 and 1: " << length(substract(all_objects[0].center, all_objects[0].get_prior_pairwise_distance()[1].first)) << endl;
		for (int i = 0; i < centers.size(); ++i){
			cout << "Object " << i << " coordinates: (" << centers[i].x << ", " << centers[i].y << ")" << endl;
			cout << "Object orientation in degrees: " << radians_to_degrees(theta[i]) << endl;
			cout << "Prior distance: " << prior_dis[i] << endl;
			cout << "Prior angle: " << prior_angle[i] << endl;
		}
	}

	//draw a graph
	/*void optimize::drawGraph(vector<double> xs, vector<double> ys){
		SimpleGraph* plot = SimpleGraph::getInstance();
		((MatPlot*)plot)->is_debug1 = true;
		plot->setData(xs,ys);
		plot->show();
	}*/
}