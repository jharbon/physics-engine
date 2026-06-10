#include <sim_utils.hpp>

#include <fmt/core.h>

#include <stdexcept>
#include <cmath>

Particle generate_particle(
        float mass, 
        float radius, 
        const WorldBounds& bounds, 
        const Vec2& min_vel, 
        const Vec2& max_vel,
        RandomNumberGenerator& rng
) {
    // Determine position bounds based on radius padding from world borders 
    Vec2 min_pos(
        bounds.left + radius,
        bounds.bottom + radius
    );
    Vec2 max_pos(
        bounds.right - radius,
        bounds.top - radius    
    );

    // Draw random x and y position from uniform distribution
    Vec2 pos(
        rng.uniform_draw(min_pos[0], max_pos[0]),
        rng.uniform_draw(min_pos[1], max_pos[1])
    );

    // Draw random x and y velocity from uniform distribution
    Vec2 vel(
        rng.uniform_draw(min_vel[0], max_vel[0]),
        rng.uniform_draw(min_vel[1], max_vel[1])
    );

    return Particle(
        mass,
        radius,
        pos,
        vel,
        Vec2(0.0, 0.0)  // Zero acceleration; this is handled by forces elsewhere   
    );
}

std::vector<Particle> generate_multiple_particles(
        size_t n,
        float min_radius,
        float max_radius,
        float density,
        const WorldBounds& bounds,
        const Vec2& min_vel, 
        const Vec2& max_vel,
        RandomNumberGenerator& rng
) {
    float world_width = bounds.right - bounds.left;
    float world_height = bounds.top - bounds.bottom;

    std::vector<Particle> particles;
    particles.reserve(n);  // Avoid multiple reallocations 
    for (size_t i = 0; i < n; ++i) {
        // Draw radius from uniform distribution
        float radius = rng.uniform_draw(min_radius, max_radius);
        
        // Check that particle can actually fit in world
        float diameter = 2.0f * radius;
        if (diameter > world_width || diameter > world_height) {
            std::string msg = fmt::format(
                "Particle with radius {} drawn from uniform distribution [{}, {}] cannot fit in world with (width, height) = ({}, {})",
                radius,
                min_radius,
                max_radius,
                world_width, 
                world_height
            );

            throw std::runtime_error(msg.c_str());
        }

        // Get mass based on density; assume particle is 2D disc with uniformly distributed mass
        float mass = density * static_cast<float>(M_PI) * radius * radius;

        particles.push_back(generate_particle(
            mass,
            radius,
            bounds,
            min_vel,
            max_vel,
            rng
        ));
    }

    return particles;
}