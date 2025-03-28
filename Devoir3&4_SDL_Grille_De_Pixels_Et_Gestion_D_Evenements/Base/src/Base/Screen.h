#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <math.h>

#include "Polygon.h"

class Screen {
    private:
    int width;
    int height;

    uint32_t* pixels = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    public:
        Screen(); // Screen a;

        Screen(SDL_Renderer* renderer, int width, int height); // Screen* screen = new Screen(render, 254, 2365);

        ~Screen();

        bool Create(SDL_Renderer* renderer, int width, int height);

        void Clear(const Color& color);
        
        bool UpdateTexture();

        bool Present();

        bool DrawPixel(int x, int y, const Color& color);
        
        template <typename T>
        bool DrawPixel(Vector2<T> position, const Color& color);

        bool DrawLine(int xi, int yi, int xf, int yf, const Color& color);
        bool DrawWuLine(int x0, int y0, int x1, int y1, const Color& color);

        bool DrawCircle(int xc, int yc, int ray, const Color& color);

        bool DrawPolygon(Vertex2i* points, int nbPoints, const Color& color);
        bool DrawPolygon(const Polygon &p, const Color& color);
        
        bool FillPolygon(Polygon &p);
        bool FillPolygon(Polygon &p, const Color& color);
};
