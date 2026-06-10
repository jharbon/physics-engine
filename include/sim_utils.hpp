#ifndef SIM_UTILS_HPP
#define SIM_UTILS_HPP

#include <Particle.hpp>
#include <SimulationPhysics.hpp>
#include <Vec2.hpp>
#include <math_utils.hpp>

#include <vector>

Particle generate_particle(
        float mass, 
        float radius, 
        const WorldBounds& bounds, 
        const Vec2& min_vel, 
        const Vec2& max_vel,
        RandomNumberGenerator& rng
);

std::vector<Particle> generate_multiple_particles(
        size_t n,
        float min_radius,
        float max_radius,
        float density,
        const WorldBounds& bounds,
        const Vec2& min_vel, 
        const Vec2& max_vel,
        RandomNumberGenerator& rng
);

#endif