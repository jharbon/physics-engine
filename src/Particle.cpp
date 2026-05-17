#include <Particle.hpp>
#include <cmath>

Particle::Particle(
        float mass,
        float radius,            
        const Vec2& init_pos,
        const Vec2& init_vel,
        const Vec2& init_accel
) : mass{mass}, radius{radius}, pos{init_pos}, vel{init_vel}, accel{init_accel} {}

void Particle::update(float delta_t) {
    this->pos = this->pos + this->vel * delta_t + this->accel * pow(delta_t, 2) * 0.5;
    this->vel += this->accel * delta_t;
}

float Particle::get_radius() const {
    return this->radius;
}

const Vec2& Particle::get_pos() const {
    return this->pos;
}

const Vec2& Particle::get_vel() const {
    return this->vel;
}

const Vec2& Particle::get_accel() const {
    return this->accel;
}

void Particle::set_pos(const Vec2& p) {
    this->pos = p;
}

void Particle::set_vel(const Vec2& v) {
    this->vel = v;
}