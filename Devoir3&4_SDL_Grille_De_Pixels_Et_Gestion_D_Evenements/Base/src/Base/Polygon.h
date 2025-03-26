#pragma once

#include "Vector2.h"

class Polygon{
    public:
    Vector2i* points;
    int nbPoints;

    Vector2i MINCorner;
    Vector2i MAXCorner;

    Polygon() : points(new Vector2i[0]), MINCorner(0, 0), MAXCorner(0, 0), nbPoints(0){}
    Polygon(Vector2i* _points, int _nbPoints) : points(_points), nbPoints(_nbPoints){
        CalculateCenter();
    }

    void CalculateCenter(){
        if (nbPoints == 0) return;

        MINCorner = MAXCorner = points[0];
        
        for (int i = 0; i < nbPoints; i++){
            if (points[i].x < MINCorner.x) MINCorner.x = points[i].x;
            if (points[i].y < MINCorner.y) MINCorner.y = points[i].y;
            if (points[i].x > MAXCorner.x) MAXCorner.x = points[i].x;
            if (points[i].y > MAXCorner.y) MAXCorner.y = points[i].y;
        }
    }

    void AddPoint(Vector2i point, int index, bool unique = true){
        if (index < 0 || index > nbPoints) return;

        Vector2i* temp = new Vector2i[nbPoints + 1];
        for (int i = 0; i < index; i++)
            temp[i] = points[i];
        temp[index] = point;
        for (int i = index + 1; i < nbPoints + 1; i++)
            temp[i] = points[i - 1];
        delete[] points;
        points = temp;
        nbPoints++;

        if (unique)
            CalculateCenter();
    }

    void AddPoint(Vector2i point, bool unique = true){
        AddPoint(point, nbPoints, unique);
    }

    // Ajoute plusieurs points au Polygone
    template <typename... Args>
    void AddPoints(const Args&... args){
        ((AddPoint(args, false)), ...);
        CalculateCenter();
    }

    void RemovePoint(int index, bool unique = true){
        if (index < 0 || index >= nbPoints) return;

        Vector2i* temp = new Vector2i[nbPoints - 1];
        for (int i = 0; i < index; i++)
            temp[i] = points[i];
        for (int i = index; i < nbPoints - 1; i++)
            temp[i] = points[i + 1];
        delete[] points;
        points = temp;
        nbPoints--;

        if (unique)
            CalculateCenter();
    }

    // Retire plusieurs points au Polygone
    template <typename... Args>
    void RemovePoints(const Args&... args){
        ((RemovePoint(args, false)), ...);
        CalculateCenter();
    }    

    ~Polygon(){
        if (points != nullptr)
            delete[] points;
    }
};
