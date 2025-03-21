#pragma once

#include <iostream>

template <typename T>
class Vector2{
    public:
    T x;
    T y;

    Vector2(T x, T y) : x(x), y(y){}

    template <typename U>
    bool operator== (const Vector2<U>& other){
        return x == other.x && y == other.y;
    }
    
    template <typename U>
    Vector2<T> operator+ (const Vector2<U>& other){
        return Vector2<T>(x + other.x, y + other.y);
    }
    
    template <typename U>
    Vector2<T> operator- (const Vector2<U>& other){
        return Vector2<T>(x - other.x, y - other.y);
    }

    friend Vector2<T> operator-(const Vector2<T>& self){
        return Vector2<T>(-self.x, -self.y);
    }

    template <typename U>
    T operator*(const Vector2<U>& other) const{
        return other.x * x + other.y * y;
    }
    
    template <typename U>
    Vector2<T> operator*(const U& nbre) const{
        return Vector2<T>(nbre * x, nbre * y);
    }
    template <typename U>
    friend Vector2<T> operator*(const U& nbre, const Vector2<T>& self){
        return Vector2<T>(nbre * self.x, nbre * self.y);
    }

    template <typename U>
    Vector2<T> operator/(const U& nbre) const{
        return Vector2<T>(x / nbre, y / nbre);
    }

    template <typename U>
    T det(const Vector2<U>& other){
        return x * other.y - y * other.x;
    }
};


using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2d = Vector2<double>;
using Vector2u = Vector2<unsigned int>;
