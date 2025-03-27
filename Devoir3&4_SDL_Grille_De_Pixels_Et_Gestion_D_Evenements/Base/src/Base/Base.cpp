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
    
    // Polygon rectangle;
    // rectangle.AddPoints(Vector2i(250, 250), Vector2i(300, 250), Vector2i(300, 350), Vector2i(250, 350));
    
    int vx = 0;
    int vy = 0;
    int currSpeed = 1;
    int angularSpeed = 0;
    float size = 1;

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
                    size = 1.05f;
                } else if (event.key.key == SDLK_Q){
                    angularSpeed = -1;
                    size = 0.95f;
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
                    size = 1;
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
            screen.DrawLine(50, i * (height - 100) / 10 + 50, width - 50,  i * (height - 100) / 10 + 50, Color::White);
            
        transform.Translate(Vector2f(vx, vy));
        transform.Rotate(Anglef::Degrees(angularSpeed));
        // transform.Scale(Vector2f(size, size));

        Polygon temp;

        // for (int i = 0; i < rectangle.nbPoints; i++)
        //     temp.AddPoint(transform.TransformVector(Vector2f(rectangle.points[i]), Vector2f(rectangle.MINCorner + rectangle.MAXCorner) / 2));
        for (int i = 0; i < octogone.nbPoints; i++)
            temp.AddPoint(transform.TransformVector(Vector2f(octogone.points[i]), Vector2f(octogone.MINCorner + octogone.MAXCorner) / 2));

        screen.FillPolygon(temp, Color::Black);

        screen.Present();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}