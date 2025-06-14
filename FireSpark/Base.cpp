#include <SDL3/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = -0.5f;
const float PARTICLE_LIFETIME = 2.0f;
const int PARTICLE_RADIUS = 12;
const int PARTICLES_ADDED_EACH_FRAME = 20;

#include "ParticleSystem.h"

int main() {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::string title = "Fire Spark";
    bool quit = false;

    // Liste des flammes actives
    std::vector<ParticleSystem> Sources;
    float WindIntensity = 0;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer(title.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Screen screen(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT)
                quit = true;

            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if (event.button.button == SDL_BUTTON_LEFT)
                    Sources.push_back(ParticleSystem(Vector2(event.button.x, event.button.y)));
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    WindIntensity = 5;
            }

            else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE)
                Sources.clear();
        }
        Time::update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        screen.Clear(Color(57, 63, 79));

        if (!Sources.empty())
            for (int i = 0; i < Sources.size(); i++)
                Sources[i].Update(screen, WindIntensity);

        // Diminuer l'intensité du vent
        if (WindIntensity > 0.01f)
            WindIntensity *= 0.9f;
        else 
            WindIntensity = 0;

        screen.Present();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
