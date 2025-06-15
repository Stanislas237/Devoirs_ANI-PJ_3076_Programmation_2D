#pragma once

#include <random>
#include "Screen.h"
#include "Polygon.h"
#include "Time.h"


float randFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
class Particle
{
    private:
        Vector2 position;
        Vector2 speed;
        float lifeTime = 0;

        // Un polygone représentant la forme de la goutte d'eau, contient sa couleur et sa taille
        Polygon shape;

        bool IS_OUT_OF_SCREEN(){
            return position.x < 10 || position.x > SCREEN_WIDTH - 10 || position.y > SCREEN_HEIGHT - 10 ||
                position.y < 10;
        }

    public:
        bool isDead = false;

        Particle(const Vector2& pos, const Vector2& vel, const Polygon& default_shape) : position(pos), speed(vel), shape(default_shape){}

        // Déplacer le polygone, voire le fichier Polygon.h (MIN et MAXCorner pour la gestion des ovales)
        void Move()
        {
            position += speed * Time::deltaTime;
            shape.MINCorner = position;
            shape.MAXCorner = shape.MINCorner + Vector2(PARTICLE_RADIUS, PARTICLE_RADIUS);
        }

        // Dessin de la goutte d'eau, décompte du temps de vie
        void Update(Screen& screen, float WindIntensity)
        {
            // Calcul du temps de vie
            lifeTime += Time::deltaTime;
            isDead = lifeTime > PARTICLE_LIFETIME || IS_OUT_OF_SCREEN();

            // Calcul de la vitesse
            speed += Vector2(randFloat(-3.2f, 3.2f) + WindIntensity, GRAVITY * Time::deltaTime + randFloat(-2.2f, 0.9f));
            Move();

            // Interpoler la couleur
            auto newGreen = static_cast<int>(255 * (lifeTime / PARTICLE_LIFETIME));
            shape.color = Color(255, (uint8_t)newGreen, 0, (uint8_t)(255 - newGreen));

            // Dessiin
            screen.FillPolygon(shape);
        }
};
