#pragma once

#include <iostream>
#include "Vector2.h"
#include "Color.h"

template <typename T>
class Vertex2{
    public:
    Vector2<T> position{0, 0};
    Color color{Color::Black};
    
    Vertex2(){}

    Vertex2(T x, T y) : position(x, y){}

    Vertex2(T x, T y, const Color& _color) : position(x, y), color(_color){}

    Vertex2(Vector2<T> position) : position(position){}

    Vertex2(Vector2<T> position, const Color& _color) : position(position), color(_color){}

    template <typename U>
    Vertex2(Vertex2<U> vertex) : position(vertex.position), color(vertex.color){}

    // Color InterpolateColor(const Vertex2<T>* points){
    //     const T d1 = sqrtf((points[0].position - position).normsqr());
    //     const T d2 = sqrtf((points[1].position - position).normsqr());
    //     const T d3 = sqrtf((points[2].position - position).normsqr())y;
    //     const T sum = d1 + d2 + d3;
    //     float factors[3];
    //     factors[0] = 1 - (d1 / static_cast<float>(sum));
    //     factors[1] = 1 - (d2 / static_cast<float>(sum));
    //     factors[2] = 1 - (d3 / static_cast<float>(sum));
    //     float sum2 = factors[0] + factors[1] + factors[2];
    //     Color color;
    //     for (int i = 0; i < 3; i++)
    //         color += points[i].color * (factors[i] / sum2);
    //     return color;
    // }
};

using Vertex2i = Vertex2<int>;
using Vertex2f = Vertex2<float>;
