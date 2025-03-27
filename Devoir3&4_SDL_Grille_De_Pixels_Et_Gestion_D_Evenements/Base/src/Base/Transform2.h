#pragma once

#include "Mat3.h"
#include "Angle.h"

template <typename T>
class Transform2{
    private:
    Vector2<T> position;
    Angle<T> rotation = Angle<T>::Degrees(0);
    Vector2<T> scale{1, 1};
    
    bool isDirty = false;
    Mat3<T> matrix;

    void UpdateMatrix(){
        matrix = CreateTranslation(position) * CreateRotation(rotation) * CreateScale(scale);
        isDirty = false;
    }

    void UpdateMatrix(const Vector2<T>& center){
        matrix = CreateTranslation(position) * CreateTranslation(center) * CreateRotation(rotation) * CreateScale(scale) * CreateTranslation(-center);
        isDirty = false;
    }

    Mat3<T> CreateTranslation(const Vector2<T>& vector){
        Mat3<T> temp;
        temp.data[2][0] = vector.x;
        temp.data[2][1] = vector.y;

        return temp;
    }

    Mat3<T> CreateRotation(const Angle<T>& angle){
        T c = std::cos(angle.ToRadians());
        T s = std::sin(angle.ToRadians());
    
        Mat3<T> temp;
        temp.data[0][0] = c;
        temp.data[0][1] = s;
        temp.data[1][0] = -s;
        temp.data[1][1] = c;

        return temp;
    }
    
    Mat3<T> CreateScale(const Vector2<T>& scale){
        Mat3<T> temp;
        temp.data[0][0] = scale.x;
        temp.data[1][1] = scale.y;

        return temp;
    }

    public:

    Vector2<T> GetPosition() const { return position; }

    Angle<T> GetRotation() const { return rotation; }
    
    Vector2<T> GetScale() const { return position; }

    void SetPosition(const Vector2<T>& newPosition){
        position = newPosition;
        isDirty = true;
    }

    void SetRotation(const Angle<T>& newRotation){
        rotation = newRotation;
        isDirty = true;
    }
    
    void SetScale(const Vector2<T>& newScale){
        scale = newScale;
        isDirty = true;
    }

    void Translate(const Vector2<T>& vector){
        position += vector;
        isDirty = true;
    }

    void Rotate(const Angle<T>& angle){
        rotation += angle;
        isDirty = true;
    }
    
    void Scale(const Vector2<T>& factor){
        scale.x *= factor.x;
        scale.y *= factor.y;
        isDirty = true;
    }

    Vector2<T> TransformVector(const Vector2<T>& vector) {
        if (isDirty)
            UpdateMatrix();
        return matrix * vector;
    }

    Vector2<T> TransformVector(const Vector2<T>& vector, const Vector2<T>& center) {
        if (isDirty)
            UpdateMatrix(center);
        return matrix * vector;
    }

    Vertex TransformVector(const Vertex& v) {
        if (isDirty)
            UpdateMatrix();
        return Vertex(matrix * v.position, v.color);
    }

    Vertex TransformVector(const Vertex& v, const Vector2<T>& center) {
        if (isDirty)
            UpdateMatrix(center);
        return Vertex(matrix * v.position, v.color);
    }
};

using Transform2f = Transform2<float>;
