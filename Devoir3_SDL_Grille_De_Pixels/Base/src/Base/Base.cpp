#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#define M_PI 3.1415927f

static bool updateRendering = true;

uint32_t ToUint32(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return r << 24 | g << 16 | b << 8 | a;
}

bool DrawPixel(uint32_t* pixels, int width, int height, int x, int y, uint32_t pixel){
    if (pixels == NULL) return false;
    if (x < 0 || x > width || y < 0 || y > height) return false;
    pixels[(y * width) + x]= pixel;
    return true;
}

int newX(int x, int y, float xc, float yc, float cos, float sin){
    return cos * (x - xc) - sin * (y - yc) + xc;
}
int newY(int x, int y, float xc, float yc, float cos, float sin){
    return cos * (y - yc) + sin * (x - xc) + yc;
}

bool DrawPixel(uint32_t* pixels, int width, int height, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a){    
    return DrawPixel(pixels, width, height, x, y, ToUint32(r, g, b, a));
}

bool DrawLine2(uint32_t* pixels, int width, int height, int x0, int y0, int x1, int y1, uint32_t color){
    if (pixels == NULL)
        return false;
    if (x0 < 0 || x0 > width || x1 < 0 || x1 > width || y0 < 0 || y0 > height || y1 < 0 || y1 > height)
        return false;
    int ux = x1 - x0;
    int uy = y1 - y0;
    float scale = 1.0f / __max(ux, uy);
    for (float t = 0; t <= 1.0f; t += scale){
        DrawPixel(pixels, width, height, t * ux + x0, t * uy + y0, color);
    }
    return true;
}

// void DrawRect2(uint32_t* pixels, int width, int height, int x, int y, int r_width, int r_height, uint32_t color, float angle){
//     if (pixels == NULL) return false;
//     if (x < 0 || x > width || y < 0 || y > height) return false;
// }

bool DrawLine(uint32_t* pixels, int width, int height, int xi, int yi, int xf, int yf, uint32_t color){
    if (pixels == NULL)
        return false;

    if (xi < 0 || xi > width || xf < 0 || xf > width || yi < 0 || yi > height || yf < 0 || yf > height || (xi == xf && yi == yf))
        return false;

    int stepX = xi < xf ? 1 : xi > xf ? -1 : 0;
    int dx = stepX * (xf - xi);

    int stepY = yi < yf ? 1 : yi > yf ? -1 : 0;
    int dy = stepY * (yf - yi);

    float correction = (dx != 0 && dy != 0) ? static_cast<float>(__max(dx, dy)) / __min(dx, dy) - 1 : 0;
    float correctionLimit = 0;

    while ((xi != xf || stepX == 0) && (yi != yf || stepY == 0))
    {
        if (!DrawPixel(pixels, width, height, xi, yi, color))
            return false;
        
        xi += (dx >= dy || correctionLimit < 1) ? stepX : 0;
        yi += (dy >= dx || correctionLimit < 1) ? stepY : 0;
        correctionLimit += correctionLimit < 1 ? correction : -1;
        // if (correctionLimit >= 1 && dx != dy){
        //     if (dx > dy)
        //         xi += stepX;
        //     else
        //         yi += stepY;
        //     correctionLimit--;            
        //     continue;
        // }        
        // xi += stepX;
        // yi += stepY;
        // correctionLimit += correction;
    }
    return true;
}

bool DrawRect(uint32_t* pixels, int width, int height, int x, int y, int r_width, int r_height, uint32_t color){
    if (pixels == NULL) return false;
    if (x < 0 || x > width || y < 0 || y > height) return false;

    for (int i = 0; i < r_width; i++)
        for (int j = 0; j < r_height; j++)        
            if (x + i < 0 || x + i > width || y + j < 0 || y + j > height) return false;
            else pixels[((y + j) * width) + x + i] = color;
    return true;
}

bool DrawRect(uint32_t* pixels, int width, int height, int x, int y, int r_width, int r_height, int angle, uint32_t color){
    if (pixels == NULL) return false;
    if (x < 0 || x > width || y < 0 || y > height || angle < 0 || angle > 360) return false;

    float RadAngle = angle * M_PI / 180;
    float sin = sinf(RadAngle);
    float cos = cosf(RadAngle);
    float xc = x + r_width / 2.0f;
    float yc = y + r_height / 2.0f;
    
    // int new_X_Left_Top = x * cos - y * sin;
    // int new_Y_Left_Top = x * sin + y * cos;
    // int new_X_Right_Top = (x + r_width) * cos - y * sin;
    // int new_Y_Right_Top = (x + r_width) * sin + y * cos;
    // int new_X_Left_Bottom = x * cos - (y + r_height) * sin;
    // int new_Y_Left_Bottom = x * sin + (y + r_height) * cos;
    // int new_X_Right_Bottom = (x + r_width) * cos - (y + r_height) * sin;
    // int new_Y_Right_Bottom = (x + r_width) * sin + (y + r_height) * cos;

    // DrawLine(pixels, width, height, new_X_Left_Top, new_Y_Left_Top, new_X_Right_Top, new_Y_Right_Top, color);
    // DrawLine(pixels, width, height, new_X_Left_Top, new_Y_Left_Top, new_X_Left_Bottom, new_Y_Left_Bottom, color);
    // DrawLine(pixels, width, height, new_X_Left_Bottom, new_Y_Left_Bottom, new_X_Right_Bottom, new_Y_Right_Bottom, color);
    // DrawLine(pixels, width, height, new_X_Right_Bottom, new_Y_Right_Bottom, new_X_Right_Top, new_Y_Right_Top, color);
    
    // for (int j = 0; j < r_height; j++){
    //     int new_X_Left = newX(x, y + j, xc, yc, cos, sin);
    //     int new_Y_Left = newY(x, y + j, xc, yc, cos, sin);
    //     int new_X_Right = newX(x + r_width, y + j, xc, yc, cos, sin);
    //     int new_Y_Right = newY(x + r_width, y + j, xc, yc, cos, sin);

    //     if (!DrawLine(pixels, width, height, new_X_Left, new_Y_Left, new_X_Right, new_Y_Right, color))
    //         return false;
    // }
    for (int i = 0; i < r_width; i++)
        for (int j = 0; j < r_height; j++){
            int new_X = newX(x + i, y + j, xc, yc, cos, sin);
            int new_Y = newY(x + i, y + j, xc, yc, cos, sin);
            if (!DrawPixel(pixels, width, height, new_X, new_Y, color))
                return false;
        }
    return true;
}

// bool DrawSkewedRect(uint32_t* pixels, int width, int height, int x, int y, int r_width, int r_height, int angle, uint32_t color){
//     if (pixels == NULL) return false;
//     if (x < 0 || x > width || y < 0 || y > height) return false;
//     float AngleTangent = tanf(angle * M_PI / 180);
//     for (int i = 0; i < r_width; i++){
//         int nbPixelsToAddOnY = i * AngleTangent;
//         for (int j = 0; j < r_height; j++){
//             int nbPixelsToAddOnX = j * AngleTangent;
//             int newX = x + i - nbPixelsToAddOnX;
//             int newY = y + j + nbPixelsToAddOnY;         
//             if (newX < 0 || newX > width || newY < 0 || newY > height) return false;
//             else pixels[(newY * width) + newX] = color;
//         }
//     }
//     return true;
// }

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
    int angle = 0;
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
                } else if (event.key.key == SDLK_S){
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

        if (x + vx > 50 && x + vx + selfWidth < width - 50)
            x += vx;
        if (y + vy > 50 && y + vy + selfHeight < height - 50)
            y += vy;
        
        angle += angularSpeed;
        if (angle > 360)
            angle = 0;

        Clear(pixels, width, height, 255, 0, 0, 255);

        DrawRect(pixels, width, height, 50, 50, width - 100, height - 100, 0x00FF00FF);
        
        for (int i = 0; i <= 10; i++)
            DrawLine(pixels, width, height, i * (width - 100) / 10 + 50, 50, i * (width - 100) / 10 + 50, height - 50, 0xFFFFFFFF);
            
        for (int i = 0; i <= 10; i++)
            DrawLine(pixels, width, height, 50, i * (height - 100) / 10 + 50, width - 50,  i * (height - 100) / 10 + 50, 0xFFFFFFFF);

        DrawRect(pixels, width, height, x, y, selfWidth, selfHeight, angle, 0x00000000);
        // DrawLine(pixels, width, height, 150, 50, 100, 570, 0x00000000);
        // DrawLine2(pixels, width, height, 150, 50, 100, 570, 0x00000000);

        Present(texture, pixels, width);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (texture) {
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            // std::cout << "render\n";
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}