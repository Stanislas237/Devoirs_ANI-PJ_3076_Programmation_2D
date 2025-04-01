#include <SDL3/SDL.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "Screen.h"
#include "Transform2.h"

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

    Polygon octogone;
    octogone.AddPoints(Vertex2i(350, 120, Color::Cyan), Vertex2i(400, 50, Color::Green), Vertex2i(450, 50, Color::Yellow), Vertex2i(500, 120, Color::Magenta), Vertex2i(500, 170, Color::Blue), Vertex2i(450, 240, Color::Black), Vertex2i(400, 240, Color::White), Vertex2i(350, 170, Color::Red));
    
    Polygon rectangle;
    rectangle.AddPoints(Vertex2i(50, 50, Color::Green), Vertex2i(width - 50, 50, Color::Green), Vertex2i(width - 50, height - 50, Color::Green), Vertex2i(50, height - 50, Color::Green));
    
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

        screen.DrawPolygon(rectangle, Color::Green, DrawMode::Fill);
        
        for (int i = 0; i <= 10; i++)
            screen.DrawWuLine(i * (width - 100) / 10 + 50, 50, i * (width - 100) / 10 + 50, height - 50, Color::White);
            
        for (int i = 0; i <= 10; i++)
            screen.DrawWuLine(50, i * (height - 100) / 10 + 50, width - 50,  i * (height - 100) / 10 + 50, Color::White);
            
        transform.Translate(Vector2f(vx, vy));
        transform.Rotate(Anglef::Degrees(angularSpeed));
        // transform.Scale(Vector2f(size, size));

        Polygon temp;

        // for (int i = 0; i < rectangle.nbPoints; i++)
        //     temp.AddPoint(Vertex2i(transform.TransformVector(Vertex2f(rectangle.points[i]), Vector2f(rectangle.MINCorner + rectangle.MAXCorner) / 2)));
        for (int i = 0; i < octogone.nbPoints; i++){
            temp.AddPoint(Vertex2i(transform.TransformVector(Vertex2f(octogone.points[i]), Vector2f(octogone.MINCorner + octogone.MAXCorner) / 2)));
            temp.points[i].color = octogone.points[i].color;
        }

        // screen.FillPolygon(temp);
        screen.DrawPolygon(temp, Color::Black, DrawMode::Triangle_Fan);
        // screen.DrawPolygon(temp, Color::White, DrawMode::Triangles);

        screen.Present();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
