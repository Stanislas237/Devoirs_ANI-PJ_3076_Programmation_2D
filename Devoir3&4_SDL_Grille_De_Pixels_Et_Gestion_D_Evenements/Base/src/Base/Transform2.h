#pragma once

#include "Mat3.h"
#define M_PI 3.141592f

template <typename T>
class Transform2{
    public:
    Mat3<T> matrix;

    void Translate(const Vector2<T>& vector){
        Mat3<T> temp;
        temp.data[2][0] = vector.x;
        temp.data[2][1] = vector.y;

        matrix *= temp;
    }

    void Rotate(const T& angle, const Vector2<T>& center){
        Translate(center);

        float RadAngle = angle * M_PI / 180;
        float sin = sinf(RadAngle);
        float cos = cosf(RadAngle);
    
        Mat3<T> temp;
        temp.data[0][0] = cos;
        temp.data[0][1] = sin;
        temp.data[1][0] = -sin;
        temp.data[1][1] = cos;

        matrix *= temp;

        Translate(-center);
    }

    void Scale(const Vector2<T>& scale){
        Mat3<T> temp;
        temp.data[0][0] = scale.x;
        temp.data[1][1] = scale.y;

        matrix *= temp;
    }
};

using Transform2f = Transform2<float>;
