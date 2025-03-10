#include <SDL3/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 20;

void DrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int ray) {
    for (int w = 0; w < ray * 2; w++) 
        for (int h = 0; h < ray * 2; h++) {
            int dx = ray - w;
            int dy = ray - h;

            if (dx * dx + dy * dy <= ray * ray)
                SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
        }
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Erreur lors de l'initialisation de SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Grille de Pixels SDL3", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Erreur lors de la création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                running = false;

        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderClear(renderer);

        for (int x = 0; x < WINDOW_WIDTH; x += CELL_SIZE){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderLine(renderer, x, 0, x, WINDOW_HEIGHT);
            
            for (int y = 0; y < WINDOW_HEIGHT / 2; y += CELL_SIZE)
            SDL_RenderLine(renderer, 0, y, WINDOW_WIDTH, y);
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            for (int y = WINDOW_HEIGHT / 2; y < WINDOW_HEIGHT; y += CELL_SIZE)
                DrawCircle(renderer, x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 2);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// Commande de build
// g++ main.cpp -o "output\main.exe" -I "SDL3\include" -L "SDL3\mingw\lib" -lSDL3
// Commande de run
// output\main.exe
