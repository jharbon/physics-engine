#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <Particle.hpp>

#include <utility>

bool particles_colliding(const Particle& p1, const Particle& p2);
std::pair<Vec2, Vec2> solve_collision_vels(
        const float m1,
        const Vec2& u1,
        const float m2,
        const Vec2& u2,
        const Vec2& col_norm,
        const Vec2& col_tan
);
void resolve_collision(Particle& p1, Particle& p2);

#endif