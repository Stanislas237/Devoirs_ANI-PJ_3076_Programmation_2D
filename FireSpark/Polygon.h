#pragma once

#include <vector>

#include "Vector2.h"

class Polygon{
    private:
    
    // Les sommets du polygone
    std::vector<Vector2> points;

    public:
    Color color;

    // Coin supérieur gauche
    Vector2 MINCorner;

    // Coin inférieur droit
    Vector2 MAXCorner;
    
    Polygon() : points(0), MINCorner(0, 0), MAXCorner(0, 0), color(Color(0, 0, 0, 255)){}
    
    Polygon(const Color& _color) : points(0), MINCorner(0, 0), MAXCorner(0, 0), color(_color){}
    
    size_t size() const { return points.size(); }
    
    // Calcul du centre du polygone
    void CalculateCenter(){
        if (points.size() == 0) return;
        
        MINCorner = MAXCorner = points[0];
        
        for (int i = 0; i < points.size(); i++){
            if (points[i].x < MINCorner.x) MINCorner.x = points[i].x;
            if (points[i].y < MINCorner.y) MINCorner.y = points[i].y;
            if (points[i].x > MAXCorner.x) MAXCorner.x = points[i].x;
            if (points[i].y > MAXCorner.y) MAXCorner.y = points[i].y;
        }
    }

    // Ajoute plusieurs points au Polygone
    template <typename... Args>
    void AddPoints(const Args&... args){
        (points.emplace_back(args), ...);
        CalculateCenter();
    }

    // Retire un point au polygone
    void RemovePoint(int index){
        if (index < 0 || index >= points.size()) return;
        points.erase(points.begin() + index);
        CalculateCenter();
    }

    // Réinitialise les points du code
    void Clear(){
        points.clear();
        MINCorner = MAXCorner = Vector2(0, 0);
    }
    
    // Est-ce que le point appartient au polygone ?
    bool ContainsPoint(const Vector2& point){
        int nbPoints = points.size();
        for (int i = 0; i < nbPoints; i++){
            const int det = (point - points[i]).det(points[(i + 1) % nbPoints] - points[i]);
            if (det > 0)
                return false;
        }
        return true;
    }

    // Destructeur de la classe
    ~Polygon(){
        points.clear();
        points.shrink_to_fit();
        MINCorner = MAXCorner = Vector2(0, 0);
    }
};
