#ifndef __COLOR_H__
#define __COLOR_H__

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
        a = c.a;
    }

    Color operator+(const Color& c){
        return Color(r + c.r, g + c.g, b + c.b, a);
    }
    void operator+=(const Color& c){
        r += c.r;
        g += c.g;
        b += c.b;
    }

    Color operator*(float nbre){
        return Color(r * nbre, g * nbre, b * nbre, a);
    }

    Color operator/(float nbre){
        return Color(r / nbre, g / nbre, b / nbre, a);
    }

    // Predefined colors
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Transparent;

    // Color::Black
};

#endif // COLOR_H