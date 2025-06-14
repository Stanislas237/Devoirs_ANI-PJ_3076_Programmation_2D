#pragma once

#include <iostream>

#include "Color.h"

class Vector2{
    public:
    float x;
    float y;

    // Constructeurs
    Vector2() : x(0), y(0){}
    Vector2(float _x, float _y) : x(_x), y(_y){}
    Vector2(const Vector2& vector) : x(vector.x), y(vector.y){}

    bool operator== (const Vector2& other){
        return x == other.x && y == other.y;
    }

    Vector2& operator=(const Vector2& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    
    Vector2 operator+ (const Vector2& other){
        return Vector2(x + other.x, y + other.y);
    }

    void operator+= (const Vector2& other){
       x += other.x;
       y += other.y;
    }
    
    Vector2 operator- (const Vector2& other) const{
        return Vector2(x - other.x, y - other.y);
    }

    friend Vector2 operator-(const Vector2& self){
        return Vector2(-self.x, -self.y);
    }

    float operator*(const Vector2& other) const{
        return other.x * x + other.y * y;
    }
    
    Vector2 operator*(const float nbre) const{
        return Vector2(nbre * x, nbre * y);
    }

    friend Vector2 operator*(const float nbre, const Vector2& self){
        return Vector2(nbre * self.x, nbre * self.y);
    }

    Vector2 operator/(const float nbre) const{
        return Vector2(x / nbre, y / nbre);
    }

    void operator/=(const float nbre){
       x /= nbre;
       y /= nbre;
    }

    // Déterminant
    float det(const Vector2& other){
        return x * other.y - y * other.x;
    }

    // Carré de la norme
    float normsqr() const{
        return x * x + y * y;
    }
};
