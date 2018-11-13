#ifndef __SIMPLEGRAPH_H
#define __SIMPLEGRAPH_H

#define _USE_MATH_DEFINES

#include <vector>

using namespace std;
#include "matplotpp.h"

class SimpleGraph : public MatPlot
{ 
public:
	enum Colors  { red, green, blue, yellow, cyan, magenta, black };
	enum PlotTypes { solid, cross }; //solid line or single points
	static SimpleGraph* getInstance(); //use this to get a pointer to the Graph-Window

	// This function shows the window
	// Call it, once all data is set
	// Closing the window will continue program execution
	void show();

	// set a data-series
	// eg. sampling points X and corresponding function values Y
	void setData( const vector<double> & X, const vector<double> & Y );

	// add a data-series
	// eg. sampling points X and corresponding function values Y
	void addData( const vector<double> & X, const vector<double> & Y, PlotTypes m = solid, Colors c = blue );

	// set the X-axis to be logarithmically scaled (default: false)
	void setLogX( bool state );

	// mark the given X-interval position with a dashed vertical line
	void markPosition( double position, Colors color = black );

	// add a line from (x1,y1) to (x2,y2) with optional color
	void addLine( double x1, double y1, double x2, double y2, Colors color = black );

	// clear all lines and datapoints
	void clear();


// DO NOT CONSIDER ANYTHING BELOW THIS LINE AS IT IS PRIVATE!
private:
	SimpleGraph();

	// This structure describes a graph of a single real function
	struct Graph{
		// The x- and y-coordinates of the points on the graph
		std::vector<double> x,y;
		// The type of the plot
		PlotTypes type;
		// The color of the line used for display
		Colors color;
	};
	// This structure describes a single straight line segment
	struct LineSegment{
		// The starting point of the line
		double x1, y1;
		// The end point of the line
		double x2, y2;
		// The display color of the line segment
		Colors color;
	};

	void DISPLAY();
	std::vector<Graph> series;
	std::vector<LineSegment> lines;
	std::vector<std::pair<double,Colors> > markers;
	bool logX;
	int left, top, width, height;

	static void display();
	static void reshape(int w,int h);
	static SimpleGraph* instance;
};

#endif
