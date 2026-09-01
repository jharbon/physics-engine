#include <SimulationPhysics.hpp>
#include <collision.hpp>

#include <stdexcept>
#include <sstream>
#include <utility>

namespace {
    void validate_step_time(float delta_t) {
        if (delta_t <= 0) { 
            std::ostringstream oss;
            oss << "Invalid value '" << delta_t << "' for SimulationPhysics 'delta_t' argument. Value must be greater than zero";
            throw std::runtime_error(oss.str()); 
        } 
    }
}

SimulationPhysics::SimulationPhysics(std::vector<Particle> particles, WorldBounds bounds)
        : particles{std::move(particles)}, bounds{bounds}, stats{0, 0, 0, 0, 0} {

    this->init_particles = this->particles;
}

void SimulationPhysics::step(float delta_t) {
    validate_step_time(delta_t);
    // Integration and wall-bounce
    for (auto& p : particles) {
        p.update(delta_t);

        // Work on local mutable copies and set at the end
        Vec2 pos = p.get_pos();
        Vec2 vel = p.get_vel(); 
        const float r = p.get_radius();
        // Check if particle has hit a wall and implement bounce mechanic
        if (pos[0] - r < this->bounds.left) {
            // Left wall
            pos[0] = this->bounds.left + r;
            vel[0] *= -1;
            ++this->stats.wall_collisions;
        }
        else if (pos[0] + r > this->bounds.right) {
            // Right wall
            pos[0] = this->bounds.right - r;
            vel[0] *= -1;
            ++this->stats.wall_collisions;
        }
        if (pos[1] - r < this->bounds.bottom) {
            // Bottom wall
            pos[1] = this->bounds.bottom + r;
            vel[1] *= -1;
            ++this->stats.wall_collisions;
        }
        else if (pos[1] + r > this->bounds.top) {
            // Top wall
            pos[1] = this->bounds.top - r;
            vel[1] *= -1;
            ++this->stats.wall_collisions;
        }

        p.set_pos(pos);
        p.set_vel(vel);
    }

    // Particle collisions
    for (size_t i = 0; i < particles.size(); i++)    
        for (size_t j = i + 1; j < particles.size(); j++) {  // Resolve each pair once
            auto& p1 = particles[i];
            auto& p2 = particles[j];

            ++this->stats.collision_checks;

            if (particles_colliding(p1, p2)) {
                ++this->stats.collisions;
                resolve_collision(p1, p2);
            }
        }

    ++this->stats.steps;
    this->stats.elapsed_time += delta_t;
}

void SimulationPhysics::reset() {
    this->particles = this->init_particles;
    this->stats = {0, 0, 0, 0, 0};
}

void SimulationPhysics::spawn_particle(const Particle& p) {
    this->particles.push_back(p);
}

const Particle& SimulationPhysics::get_particle(size_t i) const {
    return this->particles.at(i);
}

const std::vector<Particle>& SimulationPhysics::get_all_particles() const {
    return this->particles;
}

std::size_t SimulationPhysics::get_num_particles() const {
    return this->particles.size();
}

const WorldBounds& SimulationPhysics::get_bounds() const {
    return this->bounds;
}

const SimulationStatistics& SimulationPhysics::get_stats() const {
    return this->stats;
};