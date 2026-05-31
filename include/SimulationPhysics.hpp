#ifndef SIMULATION_PHYSICS_HPP
#define SIMULATION_PHYSICS_HPP

#include <Particle.hpp>

#include <vector>

struct WorldBounds {
    float left;
    float right;
    float bottom;
    float top;
};

class SimulationPhysics {
    private: 
        std::vector<Particle> particles;
        WorldBounds bounds;

        // Keep copy of initial states of particles to enable reset functionality
        std::vector<Particle> init_particles;

    public:
        SimulationPhysics(std::vector<Particle> particles, WorldBounds bounds);

        void step(float delta_t);
        void reset();

        const Particle& get_particle(size_t i) const;
        const std::vector<Particle>& get_all_particles() const;
        std::size_t get_num_particles() const;
        WorldBounds get_bounds() const;
};

#endif