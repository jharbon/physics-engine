#include <Renderer.hpp>
#include <Particle.hpp>
#include <Vec2.hpp>
#include <SimulationPhysics.hpp>
#include <SimulationControl.hpp>

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

constexpr float MASS = 0.1;
constexpr float RADIUS = 0.2;
constexpr float G_ACCEL = 9.81;  // m/s^2
constexpr double SIM_DELTA_T = 1.0 / 60.0;  // s - corresponds to 60Hz 
constexpr double FRAME_TIME_CLAMP = 0.25;  // s

int main(int argc, char* argv[]) {
    Renderer renderer(WIN_WIDTH, WIN_HEIGHT, RADIUS);

    std::vector<Particle> particles = {
        Particle(MASS, RADIUS, Vec2(0, 0.5), Vec2(0.5, 0.3), Vec2(0, 0)),
        Particle(MASS, RADIUS, Vec2(-0.5, 0), Vec2(-0.6, -0.2), Vec2(0, 0)),
        Particle(MASS, RADIUS, Vec2(0.5, 0), Vec2(0, 1.0), Vec2(0, 0)),
        Particle(MASS, RADIUS, Vec2(-1.0, 0), Vec2(2.0, 0), Vec2(0, 0)),
        Particle(MASS, RADIUS, Vec2(-1.0, 1.0), Vec2(0.5, -1.0), Vec2(0, 0))
    };

    WorldBounds bounds;
    bounds.left = WORLD_LEFT;
    bounds.right = WORLD_RIGHT;
    bounds.bottom = WORLD_BOTTOM;
    bounds.top = WORLD_TOP;
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

        if (!controller.get_state().is_paused()) {
            accumulator += frame_time;

            while (accumulator >= SIM_DELTA_T) {
                simulation.step(SIM_DELTA_T);
                accumulator -= SIM_DELTA_T;
            }
        }

        renderer.clear(0.5f, 0.5f, 0.5f, 1.0f);
        renderer.draw(simulation.get_bounds().right, simulation.get_all_particles());
    }

    return 0;
}