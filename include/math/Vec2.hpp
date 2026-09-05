#ifndef VEC2_HPP
#define VEC2_HPP

#include <cstddef>

class Vec2 {
    private: 
        float x, y;
    
    public:
        Vec2(float x = 0.0f, float y = 0.0f);

        float& operator[](std::size_t i);
        const float& operator[](std::size_t i) const;
        float mag() const;
        Vec2 norm() const;
        Vec2 operator*(float s) const;
        Vec2 operator/(float s) const;

        Vec2 operator+(const Vec2& rhs) const;
        Vec2 operator-(const Vec2& rhs) const;
        Vec2& operator+=(const Vec2& rhs);
        float dot(const Vec2& rhs) const;
};

#endif