#ifndef _GRAMMARENGINE_H_
#define _GRAMMARENGINE_H_

//#include "ShapeGrammar.h"
#include "glm\glm.hpp"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class GrammarEngine
{
public:
    GrammarEngine();
    //GrammarEngine(ShapeGrammar* sg);
    ~GrammarEngine();

    enum ShapeState{
        terminal = 0,
        nonTerminal = 1
    };

    enum Shape{
        cube = 0,
        cylinder = 1,
        sphere = 2
    };

    struct ShapeInfo{
        glm::mat4 transInfo;
        Shape type;
        bool active;
        // 0 = cube;
        // 1 = cylinder
        // 2 = sphere
    };

    void debug();
    void addShape(glm::mat4&, ShapeState, Shape);
    ShapeInfo& selectNextShape();
    void drawShapes();

private:
    //ShapeGrammar* shapeGrammar;

    vector<ShapeInfo> nonTerminals;
    vector<ShapeInfo> terminals;

};

#endif
