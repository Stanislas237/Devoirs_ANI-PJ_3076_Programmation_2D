#pragma once

#include <cstdint>

struct Color {
    uint8_t r, g, b, a;

    // Constructor
    Color() : r(0), g(0), b(0), a(255) {}
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
    Color(uint32_t color) : r(static_cast<uint8_t>((color >> 24) & 0xFF)), g(static_cast<uint8_t>((color >> 16) & 0xFF)), b(static_cast<uint8_t>((color >> 8) & 0xFF)), a(static_cast<uint8_t>(color & 0xFF)) {}

    // Destructor
    ~Color() {}

    // Convert to uint32_t (RGBA format)
    uint32_t Touint32() const {
        return (static_cast<uint32_t>(r) << 24) |
               (static_cast<uint32_t>(g) << 16) |
               (static_cast<uint32_t>(b) << 8) |
               static_cast<uint32_t>(a);
    }

    // Cast operator to uint32_t
    operator uint32_t() const {
        return Touint32();
    }

    void operator=(const Color& c){
        r = c.r;
        g = c.g;
        b = c.b;
    }

    Color operator+(const Color& c){
        return Color(r + c.r, g + c.g, b + c.b, 255);
    }
    void operator+=(const Color& c){
        r += c.r;
        g += c.g;
        b += c.b;
    }

    Color operator*(float nbre) const{
        return Color(r * nbre, g * nbre, b * nbre, 255);
    }

    void operator*=(float nbre){
        r *= nbre;
        g *= nbre;
        b *= nbre;
    }

    // Interpoler la couleur en fonction de a
    void Interpolate(uint32_t& initial_color) const{
        float alpha = a / 255.0f;
        Color startColor = Color(initial_color) * (1.0f - alpha);
        Color endColor = *this * alpha;


        initial_color = startColor + endColor;
    }

    static const Color red;
    static const Color yellow;
};
