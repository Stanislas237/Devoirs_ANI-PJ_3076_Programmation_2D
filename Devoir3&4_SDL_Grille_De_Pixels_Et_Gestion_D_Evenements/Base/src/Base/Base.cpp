#include <SDL3/SDL.h>
#include <iostream>
#include <stdio.h>
#include <string>
#define M_PI 3.141592f

#include "Vertex.h"
#include "Transform2.h"
#include "Polygon.h"

int main() {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool quit = false;

    int width = 800;
    int height = 600;
    std::string title = "SDL3Demo";

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer(title.c_str(), width, height, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    Screen screen(renderer, width, height);
    Transform2f transform;
    
    int selfWidth = 20;
    int selfHeight = 50;
    int x = width / 2;
    int y = height / 2;

    Polygon octogone;
    octogone.AddPoints(Vector2i(350, 120), Vector2i(400, 50), Vector2i(450, 50), Vector2i(500, 120), Vector2i(500, 170), Vector2i(450, 240), Vector2i(400, 240), Vector2i(350, 170));

    int vx = 0;
    int vy = 0;
    int currSpeed = 1;
    int angularSpeed = 0;

    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }

            
            if (event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.key == SDLK_ESCAPE){
                    quit = true;
                }

                if (event.key.key == SDLK_UP){
                    vy = -currSpeed;
                } else if (event.key.key == SDLK_DOWN){
                    vy = currSpeed;
                }
                
                if (event.key.key == SDLK_LEFT){
                    vx = -currSpeed;
                } else if (event.key.key == SDLK_RIGHT){
                    vx = currSpeed;
                }

                if (event.key.key == SDLK_Z){
                    if (currSpeed < 10)
                        currSpeed++;
                } else if (event.key.key == SDLK_S){
                    if (currSpeed > 1)
                        currSpeed--;
                }

                if (event.key.key == SDLK_D){
                    angularSpeed = 1;                
                } else if (event.key.key == SDLK_Q){
                    angularSpeed = -1;
                }
            }
            else if (event.type == SDL_EVENT_KEY_UP){
                if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN){
                    vy = 0;
                }
                
                if (event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT){
                    vx = 0;
                }

                if (event.key.key == SDLK_Q || event.key.key == SDLK_D){
                    angularSpeed = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        screen.Clear(Color(255, 0, 0, 255));

        screen.DrawRect(50, 50, width - 100, height - 100, Color::Green);
        
        for (int i = 0; i <= 10; i++)
            screen.DrawLine(i * (width - 100) / 10 + 50, 50, i * (width - 100) / 10 + 50, height - 50, Color::White);
            
        for (int i = 0; i <= 10; i++)
            screen.DrawWuLine(50, i * (height - 100) / 10 + 50, width - 50,  i * (height - 100) / 10 + 50, Color::White);
            
        // screen.DrawWuLine(550, 200, 50,  400, Color::Red);
        // screen.DrawLine(590, 200, 90,  400, Color::Red);
        // screen.DrawWuLine(500, 200, 250,  450, Color::Blue);
        // screen.DrawLine(570, 200, 320,  450, Color::Blue);
        // screen.DrawWuLine(450, 200, 450,  450, Color::Yellow);
        // screen.DrawLine(430, 200, 430,  450, Color::Yellow);
        // screen.DrawWuLine(400, 200, 650,  450, Color::Cyan);
        // screen.DrawLine(370, 200, 620,  450, Color::Cyan);
        // screen.DrawWuLine(350, 200, 750,  400, Color::Magenta);
        // screen.DrawLine(330, 200, 730,  400, Color::Magenta);

        transform.Translate(Vector2f(vx, vy));
        transform.Rotate(angularSpeed, Vector2f(octogone.MINCorner + octogone.MAXCorner) / 2);

        for (int i = 0; i < octogone.nbPoints; i++)
            octogone.points[i] = Vertex(octogone.points[i].x, octogone.points[i].y).Apply(transform.matrix);

        octogone.CalculateCenter();
        screen.DrawPolygon(octogone, Color::Black);

        Vector2f center = (octogone.MINCorner + octogone.MAXCorner) / 2;
        screen.DrawPixel(center, Color::White);
        // std::cout << "Center: " << center.x << " " << center.y << std::endl;
        
        screen.Present();
        transform.matrix.Reset();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}