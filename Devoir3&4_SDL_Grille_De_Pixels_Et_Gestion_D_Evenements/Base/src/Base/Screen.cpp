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

    int index = y * width + x;
    float intensity = color.a / 255.0f;
    
    Color c1(color);
    c1 *= intensity;
    c1.a = 255;
    Color c2(pixels[index]);
    c2 *= (1 - intensity);
    pixels[index] = (uint32_t)(c1 + c2);
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

    int dx = xf - xi;
    int dy = yf - yi;
    bool EXCHANGE_SIDES = abs(dy) > abs(dx);

    if (EXCHANGE_SIDES) {
        std::swap(xi, yi);
        std::swap(xf, yf);
        std::swap(dx, dy);
    }

    int stepX = (xi < xf) ? 1 : -1;
    float gradient = 0.0f;

    if (dx == 0)
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

template <typename T>
bool Screen::DrawLine(Vector2<T> start, Vector2<T> end, const Color& color) {
    return DrawLine(start.x, start.y, end.x, end.y, color);
}

template <typename T>
bool Screen::DrawWuLine(Vector2<T> start, Vector2<T> end, const Color& color) {
    return DrawWuLine(start.x, start.y, end.x, end.y, color);
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

    // PRIVATE METHODS

bool Screen::DrawPolygonLineStrip(Vertex2i* points, int nbPoints, const Color& color){
    if (nbPoints < 3)
        return false;
    for (int i = 0; i < nbPoints - 1; i++){
        if (!DrawLine(points[i].position, points[(i + 1) % nbPoints].position, color))
            return false;
    }
    return true;
}

bool Screen::DrawPolygonLines(Vertex2i* points, int nbPoints, const Color& color){
    if (nbPoints < 2)
        return false;
    for (int i = 0; i < nbPoints; i += 2){
        if (!DrawLine(points[i].position, points[(i + 1) % nbPoints].position, color))
            return false;
    }
    return true;
}

bool Screen::DrawPolygonLineLoop(Vertex2i* points, int nbPoints, const Color& color){
    if (nbPoints < 2)
        return false;
    for (int i = 0; i < nbPoints; i++){
        if (!DrawLine(points[i].position, points[(i + 1) % nbPoints].position, color))
            return false;
    }
    return true;
}

bool Screen::DrawPolygonPoints(Vertex2i *points, int nbPoints)
{
    for (int i = 0; i < nbPoints; i++){
        if (!DrawPixel(points[i].position, points[i].color))
            return false;
    }
    return true;
}

bool Screen::FillPolygon(Polygon& p, const Color& color)
{
    if (p.nbPoints < 3)
        return false;
    for (int i = p.MINCorner.x; i < p.MAXCorner.x; i++)
        for (int j = p.MINCorner.y; j < p.MAXCorner.y; j++){
            Vector2i point(i, j);
            if (p.ContainsPoint(point))
                if (!DrawPixel(point, color))
                    return false;
        }
    return true;
}

bool Screen::DrawPolygonTriangles(Vertex2i* points, int nbPoints, const Color& color){
    if (nbPoints < 3)
        return false;
    for (int i = 0; i < nbPoints; i += 3)
        for (int j = 0; j < 3; j++){
            int stamp = nbPoints % 3 == 0 ? 0 : 1;
            if (i + j >= nbPoints - stamp)
                return false;
            if (!DrawLine(points[i + j].position, points[i + ((j + 1) % 3)].position, color))
                return false;
        }
    return true;
}

bool Screen::DrawPolygonTriangleStrip(Vertex2i* points, int nbPoints, const Color& color){
    if (nbPoints < 3)
        return false;
    for (int i = 0; i < nbPoints - 2; i++){
        if (!DrawLine(points[i].position, points[i + 1].position, color))
            return false;
        if (!DrawLine(points[i + 2].position, points[i + 1].position, color))
            return false;
        if (!DrawLine(points[i].position, points[i + 2].position, color))
            return false;
    }
}

bool Screen::DrawPolygonTriangleFan(Vertex2i* points, int nbPoints, const Color& color){
    if (nbPoints < 3)
        return false;
    for (int i = 1; i < nbPoints - 1; i++){
        if (!DrawLine(points[0].position, points[i + 1].position, color))
            return false;
        if (!DrawLine(points[i].position, points[i + 1].position, color))
            return false;
        if (!DrawLine(points[i].position, points[0].position, color))
            return false;
    }
}

    // PUBLIC IMPLEMENTATION

bool Screen::DrawPolygon(Polygon &p, const Color& color, DrawMode mode = DrawMode::Lines)
{
    switch (mode){
        case DrawMode::Points:
            return DrawPolygonPoints(p.points, p.nbPoints);
        case DrawMode::Lines:
            return DrawPolygonLines(p.points, p.nbPoints, color);
        case DrawMode::Line_Loop:
            return DrawPolygonLineLoop(p.points, p.nbPoints, color);
        case DrawMode::Line_Strip:
            return DrawPolygonLineStrip(p.points, p.nbPoints, color);
        case DrawMode::Triangles:
            return DrawPolygonTriangles(p.points, p.nbPoints, color);
        case DrawMode::Triangle_Strip:
            return DrawPolygonTriangleStrip(p.points, p.nbPoints, color);
        case DrawMode::Triangle_Fan:
            return DrawPolygonTriangleFan(p.points, p.nbPoints, color);
        case DrawMode::Fill:
            return FillPolygon(p, color);
        default:
            return false;
    }
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
