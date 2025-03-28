#pragma once

#include "Vertex2.h"

class Polygon{    
    public:
    int nbPoints;
    Vertex2i* points = nullptr;

    Vector2i MINCorner;
    Vector2i MAXCorner;

    Polygon() : points(new Vertex2i[0]), MINCorner(0, 0), MAXCorner(0, 0), nbPoints(0){}
    Polygon(Vertex2i* _points, int _nbPoints) : points(_points), nbPoints(_nbPoints){
        CalculateCenter();
    }

    void CalculateCenter(){
        if (nbPoints == 0) return;

        MINCorner = MAXCorner = points[0].position;
        
        for (int i = 0; i < nbPoints; i++){
            if (points[i].position.x < MINCorner.x) MINCorner.x = points[i].position.x;
            if (points[i].position.y < MINCorner.y) MINCorner.y = points[i].position.y;
            if (points[i].position.x > MAXCorner.x) MAXCorner.x = points[i].position.x;
            if (points[i].position.y > MAXCorner.y) MAXCorner.y = points[i].position.y;
        }
    }

    void AddPoint(Vertex2i point, int index, bool unique = true){
        if (index < 0 || index > nbPoints) return;

        Vertex2i* temp = new Vertex2i[nbPoints + 1];
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

    void AddPoint(Vertex2i point, bool unique = true){
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

        Vertex2i* temp = new Vertex2i[nbPoints - 1];
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

    bool ContainsPoint(const Vector2i& point){
        for (int i = 0; i < nbPoints; i++){
            const int det = (point - points[i].position).det(points[(i + 1) % nbPoints].position - points[i].position);
            if (det > 0)
                return false;
        }
        return true;
    }

    Color InterpolateColor(const Vector2i& point, float* distancesPointer){
        distancesPointer = new float[nbPoints];
        float TotalFactors = 0.0f;

        for (int i = 0; i < nbPoints; i++){
            distancesPointer[i] = (points[i].position - point).normsqr();
    
            if (distancesPointer[i] == 0)
                return points[i].color;
            else
                distancesPointer[i] = 1000.0f / distancesPointer[i];
            TotalFactors += distancesPointer[i];
        }
        
        Color result;
        for (int i = 0; i < nbPoints; i++){
            float factor = distancesPointer[i] / TotalFactors;
            result += points[i].color * factor;
        }

        return result;
    }

    ~Polygon(){
        if (points != nullptr)
            delete[] points;
    }
};
