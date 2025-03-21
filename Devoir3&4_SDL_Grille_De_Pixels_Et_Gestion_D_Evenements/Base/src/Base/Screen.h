#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <math.h>
#include "Vector2.h"
#include "Color.h"

class Screen {
    private:
        uint32_t* pixels = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Renderer* renderer = nullptr;
    
    public:
        int width;
        int height;

        Screen(); // Screen a;

        Screen(SDL_Renderer* renderer, int width, int height); // Screen* screen = new Screen(render, 254, 2365);

        ~Screen();

        bool Create(SDL_Renderer* renderer, int width, int height);

        void Clear(const Color& color);
        
        bool UpdateTexture();

        bool Present();

        bool DrawPixel(int x, int y, Color color);
        bool DrawPixel(Vector2i position, Color color);
        bool DrawPixel(Vector2f position, Color color);

        bool DrawLine(int xi, int yi, int xf, int yf, const Color& color);

        bool DrawRect(int x, int y, int r_width, int r_height, const Color& color);
        bool DrawRect(int x, int y, int r_width, int r_height, int angle, const Color& color);

        bool DrawCircle(int xc, int yc, int ray, const Color& color);
};
