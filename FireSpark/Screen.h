#pragma once
#include "Polygon.h"

class Screen {
    private:
        int width;
        int height;

        uint32_t* pixels = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Renderer* renderer = nullptr;
        TTF_Font* font = nullptr;        SDL_Surface* textSurface;
        SDL_Color textColor = {255, 255, 255};

        bool DrawPixel(int x, int y, const Color& color){
            if (pixels == nullptr || texture == nullptr)
                return false;

            if (x < 0 || x > width || y < 0 || y > height)
                return false;

            color.Interpolate(pixels[y * width + x]);
            return true;
        }

        bool FillOval(int xc, int yc, int xray, int yray, const Color& color){
            if (pixels == nullptr || texture == nullptr)
                return false;

            if (xc < 0 || xc > width || yc < 0 || yc > height)
                return false;

            int comparative = xray * xray * yray * yray;

            for (int i = -xray; i <= xray; i++){
                int xValue = i * i * yray * yray;

                for (int j = -yray; j <= yray; j++)
                    if (xValue + j * j * xray * xray <= comparative)
                        if (!DrawPixel(i + xc, j + yc, color))
                            return false;
            }
            return true;
        }

        bool FillOval(Vector2 center, Vector2 rays, const Color& color){
            return FillOval(center.x, center.y, rays.x, rays.y, color);
        }

    public:
        Screen() : width(0), height(0), renderer(nullptr) {
            pixels = nullptr;
            texture = nullptr;
        }

        Screen(SDL_Renderer* renderer, int width, int height){
            Create(renderer, width, height);
        }

        ~Screen() {
            if (pixels != nullptr) delete[] pixels;
            if (texture != nullptr) SDL_DestroyTexture(texture);

            renderer = nullptr;
            pixels = nullptr;
            texture = nullptr;
        }

        bool Create(SDL_Renderer* renderer, int width, int height){
            this->width = width;
            this->height = height;

            if (pixels != nullptr) delete[] pixels;
            if (texture != nullptr) SDL_DestroyTexture(texture);
            if (this->renderer != nullptr) this->renderer = nullptr;

            this->renderer = renderer;

            if (this->renderer == nullptr) return false;
            texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

            if (this->texture == nullptr) {
                this->renderer = nullptr;
                return false;
            }

            pixels = new uint32_t[width * height]();

            if (pixels == nullptr){
                this->renderer = nullptr;
                return false;
            }

            Clear(Color(0, 0, 0, 255));

            // Charger la police
            font = TTF_OpenFont("assets/OpenSans_Condensed-BoldItalic.ttf", 24); // taille 24
            if (!font)
                std::cout << "Echec du chargement de la police\n";
            return true;
        }

        void Clear(const Color& color){
            if (pixels == nullptr) return;

            for (int i = 0; i < width * height; i++)
                pixels[i] = (uint32_t)color;
        }

        bool UpdateTexture(const char* text){
            if (pixels == nullptr || texture == nullptr) return false;

            // Fusionner le texte a la texture
            if (font){
                textSurface = TTF_RenderText_Blended(font, text, 0, textColor);
                if (textSurface->format != SDL_PIXELFORMAT_RGBA8888) {
                    SDL_Surface* converted = SDL_ConvertSurface(textSurface, SDL_PIXELFORMAT_RGBA8888);
                    SDL_DestroySurface(textSurface);
                    textSurface = converted;
                }

                uint32_t* textPixels = static_cast<uint32_t*>(textSurface->pixels);
                int pitch = textSurface->pitch / 4; // pitch en uint32_t

                for (int y = 0; y < textSurface->h; y++)

                    for (int x = 0; x < textSurface->w; x++) {
                        uint8_t a = textPixels[y * pitch + x] & 0xFF;
                        if (a == 0) continue;
                        int dstIndex = (y + TEXT_POSITION_Y) * width + x + TEXT_POSITION_X;
                        Color(255, 255, 255, a).Interpolate(pixels[dstIndex]);
                    }
            }

            if (!SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(uint32_t))){
                std::cout << "update texture error\n";
                return false;
            }
            return true;
        }

        bool Present(const char* text){
            if (!UpdateTexture(text)){
                return false;
            }

            if (!SDL_RenderTexture(renderer, texture, nullptr, nullptr)){
                std::cout << "render texture error\n";
                return false;
            }
            return true;
        }

        // Si le polygone n'a pas de points, on dessine l'ovale (ou cercle) contenu entre MINCorner et MAXCorner
        bool FillPolygon(Polygon& p)
        {
            if (p.size() == 0)
            {
                Vector2 center = (p.MINCorner + p.MAXCorner) / 2;
                Vector2 ray = center - p.MINCorner;

                return FillOval(center, ray, p.color);
            }
        }
};
