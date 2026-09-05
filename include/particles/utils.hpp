#ifndef SIM_UTILS_HPP
#define SIM_UTILS_HPP

#include <particles/Particle.hpp>
#include <physics/SimulationPhysics.hpp>
#include <math/Vec2.hpp>
#include <math/utils.hpp>

#include <vector>

Particle generate_particle(
        float min_radius,
        float max_radius,
        float world_width,
        float world_height,
        float density,  
        const WorldBounds& bounds, 
        const Vec2& min_vel, 
        const Vec2& max_vel,
        RandomNumberGenerator& rng
);

std::vector<Particle> generate_n_particles(
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