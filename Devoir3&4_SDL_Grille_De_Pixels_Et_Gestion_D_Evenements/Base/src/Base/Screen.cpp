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
bool Screen::DrawWuLine(int x0, int y0, int x1, int y1, const Color& color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    
    // Si la ligne est raide, on inverse x et y
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    
    // Si x0 > x1, on inverse les points
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient = (dx == 0) ? 1 : dy / dx;  // Gestion des divisions par 0

    float y = y0 + 0.5f;  // Position y en flottant

    for (int x = x0; x <= x1; x++) {
        int yPixel = floor(y);  // Partie entière de y
        
        float intensityUpper = y - yPixel;  // Intensité du pixel supérieur
        float intensityLower = 1 - intensityUpper;  // Intensité du pixel inférieur

        const Color ColorLower(color.r * intensityLower, color.g * intensityLower, color.b * intensityLower);
        const Color ColorUpper(color.r * intensityUpper, color.g * intensityUpper, color.b * intensityUpper);

        if (steep) {
            if (!DrawPixel(yPixel, x, ColorLower))
                return false;
            if (!DrawPixel(yPixel + 1, x, ColorUpper))
                return false;
        } else {
            if (!DrawPixel(x, yPixel, ColorLower))
                return false;
            if (!DrawPixel(x, yPixel + 1, ColorUpper))
                return false;
        }

        y += gradient;  // Incrémenter y selon la pente
    }
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
