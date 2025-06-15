const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = -0.5f;
const float PARTICLE_LIFETIME = 2.0f;
const int PARTICLE_RADIUS = 12;
const int PARTICLES_ADDED_EACH_FRAME = 5;
const int TEXT_POSITION_X = 50;
const int TEXT_POSITION_Y = 50;

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "ParticleSystem.h"

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Erreur init TTF: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::string title = "Fire Spark";

    if (!SDL_CreateWindowAndRenderer(title.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Screen screen(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Event event;
    // Liste des flammes actives
    std::vector<ParticleSystem> Sources;
    float WindIntensity;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if (event.button.button == SDL_BUTTON_LEFT)
                    Sources.push_back(ParticleSystem(Vector2(event.button.x, event.button.y)));
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    WindIntensity = (rand() % 2 == 0) ? -5 : 5;;
            }

            else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE)
                Sources.clear();
        }
        Time::update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        screen.Clear(Color(0, 0, 0));

        int TotalParticles = 0;
        if (!Sources.empty())
            for (int i = 0; i < Sources.size(); i++){
                Sources[i].Update(screen, WindIntensity);
                TotalParticles += Sources[i].particles.size();
            }

        // Diminuer l'intensité du vent
        if (std::abs(WindIntensity) > 0.01f)
            WindIntensity *= 0.9f;
        else
            WindIntensity = 0;

        screen.Present(("Nombre de particules : " + std::to_string(TotalParticles)).c_str());
        SDL_RenderPresent(renderer);
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
