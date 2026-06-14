#ifndef SIMULATION_PHYSICS_HPP
#define SIMULATION_PHYSICS_HPP

#include <Particle.hpp>

#include <iostream>
#include <vector>

struct WorldBounds {
    float left;
    float right;
    float bottom;
    float top;
};

struct SimulationStatistics {
    size_t steps;
    float elapsed_time;
    size_t wall_collisions;
    size_t collision_checks;
    size_t collisions;

    void print() const {
        std::cout << "Simulation Statistics\n";
        std::cout << "---------------------\n";
        std::cout << "-- Steps: " << this->steps << "\n";
        std::cout << "-- Elapsed time: " << this->elapsed_time << "s\n";
        std::cout << "-- Wall collisions: " << this->wall_collisions << "\n";
        std::cout << "-- Collision checks: " << this->collision_checks << "\n";
        std::cout << "-- Collisions: " << this->collisions << std::endl;
        float collision_rate = 
            this->collision_checks > 0 
            ? 100 * static_cast<float>(this->collisions) / this->collision_checks
            : 0.0;
        std::cout << "-- Collision rate: " << collision_rate << "%" << std::endl;
    }
};

class SimulationPhysics {
    private: 
        std::vector<Particle> particles;
        WorldBounds bounds;
        
        SimulationStatistics stats;

        // Keep copy of initial states of particles to enable reset functionality
        std::vector<Particle> init_particles;

    public:
        SimulationPhysics(std::vector<Particle> particles, WorldBounds bounds);

        void step(float delta_t);
        void reset();

        const Particle& get_particle(size_t i) const;
        const std::vector<Particle>& get_all_particles() const;
        std::size_t get_num_particles() const;
        const WorldBounds& get_bounds() const;
        const SimulationStatistics& get_stats() const;
};

#endif