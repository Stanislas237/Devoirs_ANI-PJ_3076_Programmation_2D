#include "Screen.h"

Screen::Screen() : width(0), height(0), renderer(nullptr) {
    pixels = nullptr;
    texture = nullptr;
}

Screen::Screen(SDL_Renderer* _renderer, int _width, int _height){
    Create(_renderer, _width, _height);
}

Screen::~Screen() {
    if (pixels != nullptr) delete[] pixels;
    if (texture != nullptr) SDL_DestroyTexture(texture);

    renderer = nullptr;
    pixels = nullptr;
    texture = nullptr;
}

bool Screen::Create(SDL_Renderer* _renderer, int _width, int _height){
    this->width = _width;
    this->height = _height;

    if (pixels != nullptr) delete[] pixels;
    if (texture != nullptr) SDL_DestroyTexture(texture);
    if (this->renderer != nullptr) this->renderer = nullptr;

    this->renderer = _renderer;

    if (this->renderer == nullptr) return false;
    texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);

    if (this->texture == nullptr) {
        this->renderer = nullptr;
        return false;
    }

    pixels = new uint32_t[_width * _height]();

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

bool Screen::DrawPixel(int x, int y, const Color& color){
    if (pixels == nullptr || texture == nullptr) return false;
    if (x < 50 || x > width - 50 || y < 50 || y > height - 50) return false;

    pixels[(y * width) + x] = (uint32_t)color;
    return true;
}

template <typename T>
bool Screen::DrawPixel(Vector2<T> position, const Color& color){
    return DrawPixel(position.x, position.y, color);
}

bool Screen::DrawLine(int xi, int yi, int xf, int yf, const Color& color){
    if (pixels == nullptr || texture == nullptr) return false;

    const int stepX = xi < xf ? 1 : xi > xf ? -1 : 0;
    const int dx = stepX * (xf - xi);

    const int stepY = yi < yf ? 1 : yi > yf ? -1 : 0;
    const int dy = stepY * (yf - yi);

    const int MAX = std::max(dx, dy);
    const int MIN = std::min(dx, dy);
    int error = 0;

    while (true)
    {
        if (!DrawPixel(xi, yi, color))
            return false;
        
        if (xi == xf && yi == yf) break;
        
        xi += (dx >= dy || error < MIN) ? stepX : 0;
        yi += (dy >= dx || error < MIN) ? stepY : 0;
        error += (error < MIN ? MAX : 0) - MIN;
    }
    return true;
}

// Fonction pour tracer une ligne avec Xiaolin Wu
bool Screen::DrawWuLine(int xi, int yi, int xf, int yf, const Color& color) {
    if (pixels == nullptr || texture == nullptr) return false;

    int dx = abs(xf - xi);
    int dy = abs(yf - yi);
    bool EXCHANGE_SIDES = dy > dx;

    if (EXCHANGE_SIDES) {
        std::swap(xi, yi);
        std::swap(xf, yf);
        std::swap(dx, dy);
    }

    int stepX = (xi < xf) ? 1 : -1;
    float gradient = 0.0f;

    if (dx == 0 || dx == dy)
        gradient = 1.0f;
    else
        gradient = static_cast<float>(dy) / dx;

    float y = yi;
    for (int x = xi; x != xf; x += stepX) {
        float intensity = y - floor(y);

        Color colorLower = color ^ (1 - intensity);
        Color colorUpper = color ^ intensity;

        if (EXCHANGE_SIDES){
            if (!DrawPixel(y, x, colorLower))
                return false;
            if (!DrawPixel(y + 1, x, colorUpper))
                return false;
        } else {
            if (!DrawPixel(x, y, colorLower))
                return false;
            if (!DrawPixel(x, y + 1, colorUpper))
                return false;
        }
        y += gradient;
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

bool Screen::DrawPolygon(Vertex2i* points, int nbPoints, const Color& color){
    for (int i = 0; i < nbPoints; i++){
        if (&points[i] == nullptr || &points[(i + 1) % nbPoints] == nullptr)
            return false;

        if (!DrawLine(points[i].position.x, points[i].position.y, points[(i + 1) % nbPoints].position.x, points[(i + 1) % nbPoints].position.y, color))
            return false;
    }
}

bool Screen::DrawPolygon(const Polygon &p, const Color &color)
{
    return DrawPolygon(p.points, p.nbPoints, color);
}

bool Screen::FillPolygon(Polygon &p)
{
    float* distancesPointer = nullptr;

    for (int i = p.MINCorner.x; i < p.MAXCorner.x; i++)
        for (int j = p.MINCorner.y; j < p.MAXCorner.y; j++){
            Vector2i point(i, j);
            if (p.ContainsPoint(point)){
                Color c = p.InterpolateColor(point, distancesPointer);
                if (!DrawPixel(point, c)){
                    delete[] distancesPointer;
                    return false;
                }
            }
        }
    delete[] distancesPointer;
    return true;
}

bool Screen::FillPolygon(Polygon &p, const Color &color)
{
    for (int i = p.MINCorner.x; i < p.MAXCorner.x; i++)
        for (int j = p.MINCorner.y; j < p.MAXCorner.y; j++){
            Vector2i point(i, j);
            if (p.ContainsPoint(point))
                if (!DrawPixel(point, color))
                    return false;
        }
    return true;
}
