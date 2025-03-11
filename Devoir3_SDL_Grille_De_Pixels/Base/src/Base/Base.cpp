#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
#include <iostream>
#include <stdio.h>
#include <new>
#include <string>

static bool updateRendering = true;

uint32_t ToUint32(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return r << 24 | g << 16 | b << 8 | a;
}

int MathAbs(int x){
    return x < 0 ? -x : x;
}

bool DrawPixel(uint32_t* pixels, int width, int height, int x, int y, uint32_t pixel){
    if (pixels == NULL) return false;
    if (x < 0 || x > width || y < 0 || y > height) return false;
    pixels[(y * width) + x]= pixel;
    return true;
}

bool DrawPixel(uint32_t* pixels, int width, int height, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a){    
    return DrawPixel(pixels, width, height, x, y, ToUint32(r, g, b, a));
}

bool DrawRect(uint32_t* pixels, int width, int height, int x, int y, int r_width, int r_height, uint32_t color){
    if (pixels == NULL) return false;
    if (x < 0 || x > width || y < 0 || y > height) return false;

    for (int i = 0; i < r_width; i++)
        for (int j = 0; j < r_height; j++)
            if (!DrawPixel(pixels, width, height, x + i, y + j, color))
                return false;
    return true;
}

bool DrawLine(uint32_t* pixels, int width, int height, int x_initial, int y_initial, int x_final, int y_final, uint32_t color){
    if (pixels == NULL)
        return false;

    if (x_initial < 0 || x_initial > width || x_final < 0 || x_final > width || y_initial < 0 || y_initial > height || y_final < 0 || y_final > height)
        return false;

    int dx = MathAbs(x_final - x_initial);
    int dy = MathAbs(y_final - y_initial);

    int stepX = x_initial < x_final ? 1 : -1;
    int stepY = y_initial < y_final ? 1 : -1;

    int delta = dx - dy;

    bool running = true;
    while (running)
    {
        DrawPixel(pixels, width, height, x_initial, y_initial, color);
        if (x_initial == x_final && y_initial == y_final)
            running = false;

        if (2 * delta > -dy){
            delta -= dy;
            x_initial += stepX;
        }

        if (2 * delta < dx){
            delta += dx;
            y_initial += stepY;
        }
    }
    return true;
}

void Clear(uint32_t *pixels, uint32_t width, uint32_t height, uint32_t color){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            pixels[width * j + i] = color;
        }
    }
}

void Clear(uint32_t *pixels, uint32_t width, uint32_t height, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++) {
            pixels[width * j + i] = (r << 24) | (g << 16) | (b << 8) | a;
        }
    }
}

void Present(SDL_Texture* texture, const Uint32* pixels, int width, int height) {
    // Verrouille la texture pour pouvoir y accéder directement
    void* pixelsPtr;
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixelsPtr, &pitch) != 0) {
        std::cerr << "Erreur lors du verrouillage de la texture : " << SDL_GetError() << std::endl;
        return;
    }

    // Calcul du nombre d'Uint32 par ligne (pitch en octets / taille d'un Uint32)
    int pitchPixels = pitch / sizeof(Uint32);
    Uint32* dst = static_cast<Uint32*>(pixelsPtr);

    // Copie du tableau de pixels dans la mémoire de la texture
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            dst[y * pitchPixels + x] = pixels[y * width + x];
        }
    }

    // Déverrouille la texture pour que le rendu puisse utiliser les nouvelles données
    SDL_UnlockTexture(texture);
}

void Present(SDL_Texture *texture, uint32_t *pixels, uint32_t width){
    if (!SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(uint32_t))){
        std::cout << "error\n";
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool quit = false;

    uint32_t width = 800;
    uint32_t height = 600;
    std::string title = "SDL3Demo";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(title.c_str(), width, height, 0, &window, &renderer) < 0) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (texture == nullptr) {
        SDL_Log("Yo Frangin la tetxure na pas ete creer: %s", SDL_GetError());
    }

    const uint32_t numPixels = width  * height;
    uint32_t *pixels = new uint32_t[numPixels];

    int x = width / 2;
    int y = height / 2;
    int selfWidth = 20;
    int selfHeight = 50;
    int vx = 0;
    int vy = 0;
    int currSpeed = 1;

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
            }
            else if (event.type == SDL_EVENT_KEY_UP){
                if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN){
                    vy = 0;
                }
                
                if (event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT){
                    vx = 0;
                }
            }
        }

        if (x + vx > 50 && x + vx + selfWidth < width - 50)
            x += vx;
        if (y + vy > 50 && y + vy + selfHeight < height - 50)
            y += vy;

        Clear(pixels, width, height, 255, 0, 0, 255);

        DrawRect(pixels, width, height, 50, 50, width - 100, height - 100, 0x00FF00FF);
        
        for (int i = 0; i <= 10; i++)
            DrawLine(pixels, width, height, i * (width - 100) / 10 + 50, 50, i * (width - 100) / 10 + 50, height - 50, 0xFFFFFFFF);
            
        for (int i = 0; i <= 10; i++)
            DrawLine(pixels, width, height, 50, i * (height - 100) / 10 + 50, width - 50,  i * (height - 100) / 10 + 50, 0xFFFFFFFF);

        DrawRect(pixels, width, height, x, y, selfWidth, selfHeight, 0x00000000);

        Present(texture, pixels, width);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (texture) {
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            //std::cout << "render\n";
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}