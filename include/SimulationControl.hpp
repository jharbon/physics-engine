#ifndef SIMULATION_CONTROL_HPP
#define SIMULATION_CONTROL_HPP

#include <GLFW/glfw3.h>

#include <string>

namespace key_bindings {
    constexpr int PAUSE = GLFW_KEY_SPACE;
    constexpr int STEP = GLFW_KEY_RIGHT;
}

class SimulationState {
    private:
        bool paused;
        bool step;

    public:
        SimulationState(bool paused = false, bool step = false);

        void toggle_pause();
        void toggle_step();

        bool is_paused() const;
        bool is_step() const;
};

class SimulationController {
    private:
        SimulationState state;

        bool pause_pressed;
        bool step_pressed;

    public:
        SimulationController(bool paused = false, bool step = false);

        void update_state(GLFWwindow* window);

        const SimulationState& get_state() const;
};

#endif