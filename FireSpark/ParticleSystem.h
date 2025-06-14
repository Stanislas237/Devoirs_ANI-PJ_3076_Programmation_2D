#pragma once

#include "Particle.h"

class ParticleSystem
{
    private:
        // La forme par défaut de chaque particule (un cercle rouge)
        Polygon default_shape = Polygon(Color::red);

        // La position de la flamme
        Vector2 position;

        // Emission de particules
        void EmitParticles()
        {
            // Ajouter nouvelles particules
            for (int i = 0; i < PARTICLES_ADDED_EACH_FRAME; i++)
                particles.push_back(Particle(position, Vector2(randFloat(-0.5f, 0.5f), randFloat(-2.0f, -1.0f)), default_shape));
        }

    public:
        // Un vector de Particules
        std::vector<Particle> particles;

        // Constructeur du système de particules
        ParticleSystem(Vector2 pos) : position(pos){
            default_shape.MAXCorner = Vector2(PARTICLE_RADIUS, PARTICLE_RADIUS);
        }

        // Emission des particules et affichage/suppression de chaque goutte de pluie
        void Update(Screen& screen, float WindIntensity)
        {
            EmitParticles();

            if (!particles.empty())
                for (int i = 0; i < particles.size(); i++)
                    if (particles[i].isDead)
                        particles.erase(particles.begin() + i);
                    else
                        particles[i].Update(screen, WindIntensity);
        }
        
        // Destructeur de la classe
        ~ParticleSystem(){
            particles.clear();
        }
};
