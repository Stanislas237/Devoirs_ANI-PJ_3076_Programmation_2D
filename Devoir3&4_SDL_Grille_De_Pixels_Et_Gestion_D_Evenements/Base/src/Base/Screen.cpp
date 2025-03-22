#include "Screen.h"
#include "Vector2.h"
#define M_PI 3.141592f

Screen::Screen() : width(0), height(0), renderer(nullptr) {
    pixels = nullptr;
    texture = nullptr;
}

Screen::Screen(SDL_Renderer* renderer, int width, int height){
    Create(renderer, width, height);
}

Screen::~Screen() {
    if (pixels != nullptr) delete[] pixels;
    if (texture != nullptr) SDL_DestroyTexture(texture);

    renderer = nullptr;
    pixels = nullptr;
    texture = nullptr;
}

bool Screen::Create(SDL_Renderer* renderer, int width, int height){
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
    return true;
}

void Screen::Clear(const Color& color) {
    if (pixels == nullptr) return;

    for (int i = 0; i < width * height; i++)
        pixels[i] = (uint32_t)color;
}

bool Screen::UpdateTexture(){
    if (pixels == nullptr || texture == nullptr) return false;

    if (!SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(uint32_t))){
        std::cout << "update texture error\n";
        return false;
    }
    return true;
}

bool Screen::Present() {
    if (!UpdateTexture()){
        return false;
    }

    if (!SDL_RenderTexture(renderer, texture, nullptr, nullptr)){
        std::cout << "render texture error\n";
        return false;
    }
    return true;
}

bool Screen::DrawPixel(int x, int y, Color color){
    if (pixels == nullptr || texture == nullptr) return false;
    if (x < 50 || x > width - 50 || y < 50 || y > height - 50) return false;

    pixels[(y * width) + x] = (uint32_t)color;
    return true;
}
bool Screen::DrawPixel(Vector2i position, Color color){
    return DrawPixel(position.x, position.y, color);
}
bool Screen::DrawPixel(Vector2f position, Color color){
    return DrawPixel(position.x, position.y, color);
}

bool Screen::DrawLine(int xi, int yi, int xf, int yf, const Color& color){
    if (pixels == nullptr || texture == nullptr) return false;
    if (xi == xf && yi == yf) return false;
    // if (xi < 0 || xi > width || xf < 0 || xf > width || yi < 0 || yi > height || yf < 0 || yf > height || (xi == xf && yi == yf)) return false;

    int stepX = xi < xf ? 1 : xi > xf ? -1 : 0;
    int dx = stepX * (xf - xi);

    int stepY = yi < yf ? 1 : yi > yf ? -1 : 0;
    int dy = stepY * (yf - yi);

    float correction = (dx != 0 && dy != 0) ? static_cast<float>(__max(dx, dy)) / __min(dx, dy) - 1 : 0;
    float correctionLimit = 0;

    while ((xi != xf || stepX == 0) && (yi != yf || stepY == 0))
    {
        if (!DrawPixel(xi, yi, color)) return false;
        
        xi += (dx >= dy || correctionLimit < 1) ? stepX : 0;
        yi += (dy >= dx || correctionLimit < 1) ? stepY : 0;
        correctionLimit += correctionLimit < 1 ? correction : -1;
    }
    return true;
}

float newX(int x, int y, float xc, float yc, float cos, float sin){
    return cos * (x - xc) - sin * (y - yc) + xc;
}
float newY(int x, int y, float xc, float yc, float cos, float sin){
    return cos * (y - yc) + sin * (x - xc) + yc;
}

bool Screen::DrawRect(int x, int y, int r_width, int r_height, const Color& color){
    if (pixels == nullptr || texture == nullptr) return false;
    if (x < 0 || x > width || y < 0 || y > height) return false;

    for (int i = 0; i < r_width; i++)
        for (int j = 0; j < r_height; j++)        
            DrawPixel(x + i, y + j, color);
    return true;
}

bool Screen::DrawRect(int x, int y, int r_width, int r_height, int angle, const Color& color){
    if (pixels == nullptr || texture == nullptr) return false;
    if (x < 0 || x > width || y < 0 || y > height || angle < 0 || angle > 360) return false;

    float RadAngle = angle * M_PI / 180;
    float sin = sinf(RadAngle);
    float cos = cosf(RadAngle);
    float xc = x + r_width / 2.0f;
    float yc = y + r_height / 2.0f;
    
    for (int i = 0; i < r_width; i++)
        for (int j = 0; j < r_height; j++){
            float new_X = newX(x + i, y + j, xc, yc, cos, sin);
            float new_Y = newY(x + i, y + j, xc, yc, cos, sin);
            if (!DrawPixel(new_X, new_Y, color))
                return false;
        }
    return true;
}

bool Screen::DrawCircle(int xc, int yc, int ray, const Color& color){
    if (pixels == nullptr || texture == nullptr) return false;
    if (xc < 0 || xc > width || yc < 0 || yc > height) return false;

    for (int i = -ray; i <= ray; i++)
        for (int j = -ray; j <= ray; j++)
            if (i * i + j * j <= ray * ray)
                if (!DrawPixel(i + xc, j + yc, color))
                    return false;
    return true;
}
