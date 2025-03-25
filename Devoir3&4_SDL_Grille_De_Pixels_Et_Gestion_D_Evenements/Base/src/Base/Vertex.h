#pragma once

#include <iostream>
#include "Screen.h"
#include "Mat3.h"

class Vertex{
    public:
    Vector2f position;
    Color color;
    
    Vertex() : position(0, 0), color(Color::Black){}
    Vertex(float x, float y) : position(x, y), color(Color::Black){}
    Vertex(float x, float y, const Color& _color) : position(x, y), color(_color){}

    void Apply(Mat3f matrix){
        position = matrix * position;        
    }
};
