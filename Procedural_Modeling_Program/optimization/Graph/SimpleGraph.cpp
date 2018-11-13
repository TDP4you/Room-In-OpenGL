#include <GL/freeglut.h>
#include "SimpleGraph.h"
#include <assert.h>
#include <tuple>

SimpleGraph* SimpleGraph::getInstance()
{
	if ( instance == NULL )
		instance = new SimpleGraph();

	return instance;
}

void SimpleGraph::show()
{
	int argi = 0;
	char* argv = new char[10];
	glutInit( &argi, &argv );
	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );
	glutCreateWindow(left, top, width, height);
	glutSetWindowTitle( "SimpleGraph Viewer -- close to continue" );
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutMainLoop();
}

void SimpleGraph::setData( const vector<double> & X, const vector<double> & Y )
{
	series.clear();
	addData( X, Y );
}

void SimpleGraph::addData( const vector<double> & X, const vector<double> & Y, PlotTypes m, Colors c )
{
	assert( X.size() == Y.size() );
	Graph newGraph={X,Y,m,c};
	series.push_back(newGraph);
}

void SimpleGraph::addLine( double x1, double y1, double x2, double y2, Colors color /*= black */ )
{
  LineSegment newLine={x1,y1,x2,y2,color};
	lines.push_back(newLine);
}


void SimpleGraph::markPosition( double position, Colors color /*= black */ )
{
	markers.push_back( std::make_pair(position, color) );
}


void SimpleGraph::setLogX( bool state )
{
	logX = state;
}

SimpleGraph::SimpleGraph()
	: logX( false )
	, left(100), top(100), width(400), height(300)
	, MatPlot()
{
}

void SimpleGraph::DISPLAY()
{
	std::string colors[] = {"r","g","b","y","c","m","k"};
	if ( !series.empty() )
	{
		for ( auto it = series.begin(); it != series.end(); ++ it )
		{
			if ( logX )
				semilogx( it->x, it->y );
			else
				plot( it->x, it->y );

			set("Color",( colors[it->color] ));
			if ( it->type == solid )
			{
				set("Marker","none");
				set("LineStyle","-");
			}
			else
			{
				set("Marker","x");
				set("LineStyle","none");
			}
		}
	}	

	std::vector<double> p1(2), p2(2);
	for( size_t i = 0; i < lines.size(); ++i )
	{
		p1[0] = lines[i].x1;
		p1[1] = lines[i].x2;
		p2[0] = lines[i].y1;
		p2[1] = lines[i].y2;
		line( p1, p2 );
		set("Color",( colors[lines[i].color] ));
	}

	for( size_t i = 0; i < markers.size(); ++i )
	{
		p1[0] = p1[1] = markers[i].first;
		p2[0] = -1;
		p2[1] = 1;
		line( p1, p2 );
		set("Color",( colors[markers[i].second] ));
		set( "LineStyle", "- -" );
	}
		
}
void SimpleGraph::display(){ ((MatPlot*)instance)->display(); }
void SimpleGraph::reshape(int w,int h){ ((MatPlot*)instance)->reshape(w,h); }

void SimpleGraph::clear()
{
	lines.clear();
	markers.clear();
	series.clear();
}

SimpleGraph* SimpleGraph::instance = NULL;
