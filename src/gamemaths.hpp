#ifndef GAME_MATH_HPP_INCLUDED
#define GAME_MATH_HPP_INCLUDED

#include <cstring>
#include <cmath>
#include <SFML/Graphics.hpp>

inline float sqrt(float x)
{
    return std::sqrt(x);
}
inline float rsqrt(float x)
{
    return 1.f / std::sqrt(x);
}
inline float sqr(float x)
{
    return x * x;
}

inline float abs(sf::Vector2f vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline sf::Vector2f normalize(sf::Vector2f vec)
{
    return vec * rsqrt(vec.x * vec.x + vec.y * vec.y);
}

inline int round(float x)
{
    return std::round(x);
}

inline const float PI = 3.14159265358979323846f;

class Angle
{
    inline static const float FULL_CYCLE = 360;

  public:
    Angle() noexcept = default;

    static Angle fromDegrees(float degrees) noexcept
    {
        Angle res;
        res.m_degrees = degrees;
        return res;
    }
    static Angle fromRadians(float radians) noexcept
    {
        Angle res;
        res.m_degrees = radianstoDegrees(radians);
        return res;
    }

    static Angle fullCycle() noexcept
    {
        return Angle::fromDegrees(FULL_CYCLE);
    }
    
    float asRadians() const noexcept 
    { 
        return degreesToRadians(this->m_degrees); 
    }
    float asDegrees() const noexcept
    { 
        return this->m_degrees; 
    }

    operator sf::Transform() const noexcept
    {
        sf::Transform transform;
        transform.rotate(this->asDegrees());
        return transform;
    }

    friend Angle operator + (Angle a) noexcept
    {
        return a;
    }

    friend Angle operator - (Angle a) noexcept
    {
        Angle res;
        res.m_degrees = -a.m_degrees;
        return res;
    }

    Angle& operator += (Angle other) noexcept
    {
        this->m_degrees += other.m_degrees;
        return *this;
    }
    Angle& operator -= (Angle other) noexcept
    {
        this->m_degrees -= other.m_degrees;
        return *this;
    }
    Angle& operator *= (float scalar) noexcept
    {
        this->m_degrees *= scalar;
        return *this;
    }
    Angle& operator /= (float scalar) noexcept
    {
        this->m_degrees /= scalar;
        return *this;
    }

  private:
    static float radianstoDegrees(float radians) noexcept
    {
        return radians * 180.f / PI;
    }
    static float degreesToRadians(float degrees) noexcept
    {
        return degrees * PI / 180.f;
    }
    // static float normalize(float radians) noexcept
    // {
    //     radians = fmod(radians, FULL_CYCLE);
    //     if (radians < 0.f)
    //       radians += FULL_CYCLE;
    //     return radians;
    // }

    float m_degrees;
};

inline Angle operator + (Angle a, Angle b) noexcept
{
    a += b;
    return a;
}
inline Angle operator - (Angle a, Angle b) noexcept
{
    a -= b;
    return a;
}
inline Angle operator * (Angle a, float scalar) noexcept
{
    a *= scalar;
    return a;
}
inline Angle operator * (float scalar, Angle a) noexcept
{
    a *= scalar;
    return a;
}
inline Angle operator / (Angle a, float scalar) noexcept
{
    a /= scalar;
    return a;
}

inline float sin(Angle angle) noexcept
{
    return std::sin(angle.asRadians());
}

inline float cos(Angle angle) noexcept
{
    return std::cos(angle.asRadians());
}

inline float tan(Angle angle) noexcept
{
    return std::tan(angle.asRadians());
}

inline sf::Vector2f toDirection(Angle angle) noexcept
{
    return sf::Vector2f(cos(angle), sin(angle));
}

inline Angle toAngle(sf::Vector2f direction) noexcept
{
    return Angle::fromRadians(std::atan2(direction.y, direction.x));
}

inline sf::Color lerp(sf::Color c1, sf::Color c2, float v1, float v2, float val)
{
    if (val == v1) 
      return c1;
    if (val == v2)
      return c2; 
    float interpolation = (val - v1) / (v2 - v1);
    if (interpolation <= 0) return c1;
    if (interpolation >= 1) return c2;
    sf::Color res;
    res.r = c1.r + (c2.r - c1.r) * interpolation;
    res.g = c1.g + (c2.g - c1.g) * interpolation;
    res.b = c1.b + (c2.b - c1.b) * interpolation;
    res.a = c1.a + (c2.a - c1.a) * interpolation;
    return res;
}

inline float lerp(float c1, float c2, float v1, float v2, float val)
{
    if (val == v1) 
      return c1;
    if (val == v2)
      return c2; 
    float interpolation = (val - v1) / (v2 - v1);
    if (interpolation <= 0) return c1;
    if (interpolation >= 1) return c2;
    return c1 + (c2 - c1) * interpolation;
}

#endif // GAME_MATH_HPP_INCLUDED