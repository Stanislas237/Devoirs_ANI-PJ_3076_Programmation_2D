#ifndef __ANGLE_H__
#define __ANGLE_H__

#include <cmath>

// Angle class for efficient angle conversions with full operator support
template<typename T>
class Angle {
public:

    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float DEG_TO_RAD = PI / 180.0f;
    static constexpr float RAD_TO_DEG = 180.0f / PI;

    static Angle<T> Degrees(T degrees) {
        return Angle<T>(degrees * DEG_TO_RAD);
    }

    static Angle<T> Radians(T radians) {
        return Angle<T>(radians);
    }

    T ToDegrees() const { return radians_ * RAD_TO_DEG; }
    T ToRadians() const { return radians_; }

    // Comparison operators
    bool operator==(const Angle& other) const { return radians_ == other.radians_; }
    bool operator!=(const Angle& other) const { return !(*this == other); }

    // Arithmetic operators
    Angle operator+(const Angle& other) const { return Radians(radians_ + other.radians_); }
    Angle operator-(const Angle& other) const { return Radians(radians_ - other.radians_); }
    Angle operator*(T scalar) const { return Radians(radians_ * scalar); }
    Angle operator/(T scalar) const { return Radians(radians_ / scalar); }

    // Compound assignment operators
    Angle& operator+=(const Angle& other) { radians_ += other.radians_; return *this; }
    Angle& operator-=(const Angle& other) { radians_ -= other.radians_; return *this; }
    Angle& operator*=(T scalar) { radians_ *= scalar; return *this; }
    Angle& operator/=(T scalar) { radians_ /= scalar; return *this; }

    // Unary operators
    Angle operator-() const { return Radians(-radians_); }

private:
    explicit Angle(T radians) : radians_(radians) {}
    T radians_;
};

// Non-member operators for scalar multiplication (left side)
template<typename T>
Angle<T> operator*(T scalar, const Angle<T>& angle) {
    return angle * scalar;
}

using Anglef = Angle<float>;
using Angled = Angle<double>;

#endif // __ANGLE_H__