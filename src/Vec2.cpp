#include <Vec2.hpp>
#include <sstream>
#include <stdexcept>
#include <cmath>

Vec2::Vec2(float x, float y) : x{x}, y{y} {}

float Vec2::operator[](std::size_t i) const {
    if (i == 0) {
        return this->x;
    }
    else if (i == 1) {
        return this->y;
    }
    else {
        std::stringstream error;
        error << "Attempted to index Vec2 with index " << i << " but only 0 or 1 are allowed";
        throw std::runtime_error(error.str());
    }
}

float Vec2::mag() const {
    return sqrt(this->x * this->x + this->y * this->y);
}

Vec2 Vec2::norm() const {
    if (this->x == 0.0 && this->y == 0.0) {
        throw std::runtime_error("Cannot compute the norm of a zero Vec2");
    }

    float vec_mag = this->mag();
    float x_out = this->x / vec_mag;
    float y_out = this->y / vec_mag;

    return Vec2(x_out, y_out);
}

Vec2 Vec2::operator*(float s) const {
    float x_out = this->x * s;
    float y_out = this->y * s;

    return Vec2(x_out, y_out);
}

Vec2 Vec2::operator/(float s) const {
    if (std::abs(s - 0.0) < 1e-5) {
        throw std::runtime_error("Cannot divide Vec2 by 0");
    }
    float x_out = this->x / s;
    float y_out = this->y / s;

    return Vec2(x_out, y_out);
}

Vec2 Vec2::operator+(const Vec2& rhs) const {
    float x_out = this->x + rhs.x;
    float y_out = this->y + rhs.y;

    return Vec2(x_out, y_out);
}

Vec2 Vec2::operator-(const Vec2& rhs) const {
    float x_out = this->x - rhs.x;
    float y_out = this->y - rhs.y;

    return Vec2(x_out, y_out);
}

Vec2& Vec2::operator+=(const Vec2& rhs) {
    this->x = this->x + rhs.x;
    this->y = this->y + rhs.y;

    return *this;
}

float Vec2::dot(const Vec2& rhs) const {
    return this->x * rhs.x + this->y * rhs.y;
}