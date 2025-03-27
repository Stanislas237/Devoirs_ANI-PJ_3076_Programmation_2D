#pragma once

#include <iostream>
#include "Vector2.h"

template <typename T>
class Mat3{
    public:
    T data[3][3];

    Mat3(){
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                data[i][j] = (i == j) ? 1 : 0;
    }

    bool operator== (const Mat3<T> other){        
        return data == other.data;
    }

    Mat3<T> operator+ (const Mat3<T>& other){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] = data[i][j] + other.data[i][j];
        return temp;
    }

    Mat3<T> operator- (const Mat3<T>& other){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] = data[i][j] - other.data[i][j];
        return temp;
    }

    // Produit par une autre matrice
    Mat3<T> operator* (const Mat3<T>& other){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] = data[0][j] * other.data[i][0] + data[1][j] * other.data[i][1] + data[2][j] * other.data[i][2];
        return temp;
    }
    void operator*= (const Mat3<T>& other){
        *this = this->operator*(other);
    }

    // Multiplication par un vecteur
    Vector2<T> operator* (const Vector2<T>& vector){
        Vector2<T> temp(0, 0);
        
        temp.x = data[0][0] * vector.x + data[1][0] * vector.y + data[2][0];
        temp.y = data[0][1] * vector.x + data[1][1] * vector.y + data[2][1];
        return temp;
    }

    // Multiplication par un scalaire
    Mat3<T> operator* (const T& nbre){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] *= nbre;
        return temp;
    }

    // Division par un scalaire
    Mat3<T> operator/ (const T& nbre){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] /= nbre;
        return temp;
    }

    Mat3<T> transpose (){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] = data[j][i];
        return temp;
    }
    
    Mat3<T> comatrice (){
        Mat3<T> temp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                temp.data[i][j] = data[(i+1)%3][(j+1)%3] * data[(i+2)%3][(j+2)%3] - data[(i+1)%3][(j+2)%3] * data[(i+2)%3][(j+1)%3];
        return temp;
    }

    T det (){
        T temp;
        for (int i = 0; i < 3; i++)
            temp += data[0][i] * (data[1][(i+1)%3] * data[2][(i+2)%3] - data[1][(i+2)%3] * data[2][(i+1)%3]);
        return temp;
    }
    
    T trace(){
        T temp;
        for (int i = 0; i < 3; i++)
            temp += data[i][i];
        return temp;
    }

    Mat3<T> inverse(){
        return comatrice().transpose() / det();
    }
};

using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;
