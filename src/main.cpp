#include <Renderer.hpp>
#include <Particle.hpp>
#include <Vec2.hpp>
#include <math_utils.hpp>
#include <sim_utils.hpp>
#include <SimulationPhysics.hpp>
#include <SimulationControl.hpp>

#include <spdlog/spdlog.h>

#include <iostream>
#include <vector>
#include <chrono>

using sc = std::chrono::steady_clock;

// Resolution
constexpr int WIN_WIDTH = 960;
constexpr int WIN_HEIGHT = 540;
// World (physics) space bounds
constexpr float WORLD_RIGHT = (float)WIN_WIDTH / WIN_HEIGHT;
constexpr float WORLD_LEFT = -WORLD_RIGHT;
constexpr float WORLD_TOP = 1;
constexpr float WORLD_BOTTOM = -WORLD_TOP;

// Particles
constexpr size_t NUM_PARTICLES = 50;
constexpr float DENSITY = 5.0;
constexpr float MIN_RADIUS = 0.05;
constexpr float MAX_RADIUS = 0.1;
const Vec2 MIN_VEL(-1.0, -1.0);
const Vec2 MAX_VEL(1.0, 1.0);

// Physics
constexpr float G_ACCEL = 9.81;  // m/s^2
constexpr double SIM_DELTA_T = 1.0 / 60.0;  // s - corresponds to 60Hz 
constexpr double FRAME_TIME_CLAMP = 0.25;  // s

int main(int argc, char* argv[]) {
    Renderer renderer(WIN_WIDTH, WIN_HEIGHT);
    spdlog::info("Window renderer created with resolution {}x{}", WIN_WIDTH, WIN_HEIGHT);

    WorldBounds bounds;
    bounds.left = WORLD_LEFT;
    bounds.right = WORLD_RIGHT;
    bounds.bottom = WORLD_BOTTOM;
    bounds.top = WORLD_TOP;

    RandomNumberGenerator rng;
    std::vector<Particle> particles = generate_n_particles(
        NUM_PARTICLES,
        MIN_RADIUS,
        MAX_RADIUS,
        DENSITY,
        bounds,
        MIN_VEL,
        MAX_VEL,
        rng
    );
    spdlog::info(
        "Generated {} particles with radius drawn from [{},{}]m, density {} kgm^-3, x-velocity drawn from [{},{}]ms^-1, and y-velocity drawn from [{},{}]ms^-1",
        particles.size(),
        MIN_RADIUS,
        MAX_RADIUS,
        DENSITY,
        MIN_VEL[0],
        MAX_VEL[0],
        MIN_VEL[1],
        MAX_VEL[1]
    );

    SimulationPhysics simulation(particles, bounds);
    SimulationController controller;
    
    auto last = sc::now();
    auto current = sc::now();
    double frame_time;
    double accumulator = 0.0;

    // Run main loop until window is closed
    while (!renderer.should_close()) {
        // Check for new events
        renderer.poll_events();
        controller.update_state(renderer.get_window());
        
        current = sc::now();
        frame_time = std::chrono::duration<double>(current - last).count();
        last = current;

        // Clamp very large frame times which could occur due to stutter, user interaction, etc...
        if (frame_time > FRAME_TIME_CLAMP) {
            frame_time = FRAME_TIME_CLAMP;
        }

        if (controller.get_state().is_reset()) {
            simulation.reset();
            spdlog::info("Simulation reset");
            accumulator = 0.0;
        }

        if (!controller.get_state().is_paused()) {
            // Scale frame time to control speed of simulation
            accumulator += frame_time * controller.get_state().get_time_scale();

            while (accumulator >= SIM_DELTA_T) {
                simulation.step(SIM_DELTA_T);
                accumulator -= SIM_DELTA_T;
            }
        }

        renderer.clear(0.5f, 0.5f, 0.5f, 1.0f);
        renderer.draw(simulation.get_bounds().right, simulation.get_all_particles());
    }

    simulation.get_stats().print();

    return 0;
}