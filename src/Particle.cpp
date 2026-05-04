#include <Particle.hpp>
#include <cmath>

Particle::Particle(            
        const Vec2& init_pos,
        const Vec2& init_vel,
        const Vec2& init_accel
) : pos{init_pos}, vel{init_vel}, accel{init_accel} {}

void Particle::update(float delta_t) {
    this->pos = this->pos + this->vel * delta_t + this->accel * pow(delta_t, 2) * 0.5;
    this->vel += this->accel * delta_t;
}

Vec2 Particle::get_pos() const {
    return this->pos;
}

Vec2 Particle::get_vel() const {
    return this->vel;
}

Vec2 Particle::get_accel() const {
    return this->accel;
}