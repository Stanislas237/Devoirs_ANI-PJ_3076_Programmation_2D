#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <math.h>

#include "Polygon.h"
#include "DrawMode.h"

class Screen {
    private:
        int width;
        int height;

        uint32_t* pixels = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Renderer* renderer = nullptr;
        
        bool DrawPolygonPoints(Vertex2i* points, int nbPoints);
        
        bool DrawPolygonLines(Vertex2i* points, int nbPoints, const Color& color);
        bool DrawPolygonLineStrip(Vertex2i* points, int nbPoints, const Color& color);
        bool DrawPolygonLineLoop(Vertex2i* points, int nbPoints, const Color& color);
        bool DrawPolygonTriangles(Vertex2i* points, int nbPoints, const Color& color);
        bool DrawPolygonTriangleStrip(Vertex2i* points, int nbPoints, const Color& color);
        bool DrawPolygonTriangleFan(Vertex2i* points, int nbPoints, const Color& color);

        bool FillPolygon(Polygon& p, const Color& color);
    
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
        
        bool DrawWuLine(int x0, int y0, int x1, int y1, const Color& color);
        bool DrawLine(int xi, int yi, int xf, int yf, const Color& color);
        
        template <typename T>
        bool DrawLine(Vector2<T> start, Vector2<T> end, const Color& color);
        
        template <typename T>
        bool DrawWuLine(Vector2<T> start, Vector2<T> end, const Color& color);
        
        bool DrawCircle(int xc, int yc, int ray, const Color& color);

        bool DrawPolygon(Polygon &p, const Color& color, DrawMode mode);
        
        bool FillPolygon(Polygon &p);
};
