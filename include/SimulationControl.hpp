#ifndef SIMULATION_CONTROL_HPP
#define SIMULATION_CONTROL_HPP

#include <GLFW/glfw3.h>

#include <string>

namespace sim_control {
    inline constexpr float DEFAULT_TIME_SCALE = 1.0;
    inline constexpr float MIN_TIME_SCALE = 0.125;
    inline constexpr float MAX_TIME_SCALE = 8.0;
}

namespace key_bindings {
    constexpr int PAUSE = GLFW_KEY_SPACE;
    constexpr int STEP = GLFW_KEY_RIGHT;
    constexpr int RESET = GLFW_KEY_R;
    constexpr int HALF_TIME_SCALE = GLFW_KEY_COMMA;
    constexpr int DOUBLE_TIME_SCALE = GLFW_KEY_PERIOD;
    constexpr int RESET_TIME_SCALE = GLFW_KEY_SLASH;
}

class SimulationState {
    private:
        bool paused;
        bool step;
        bool reset;

        float time_scale;
        float min_time_scale;
        float max_time_scale;

    public:
        SimulationState(
                bool paused = false,
                bool step = false, 
                float time_scale = sim_control::DEFAULT_TIME_SCALE, 
                float min_time_scale = sim_control::MIN_TIME_SCALE, 
                float max_time_scale = sim_control::MAX_TIME_SCALE
        );

        void toggle_pause();
        void half_time_scale();
        void double_time_scale();
        void reset_time_scale();

        bool is_paused() const;
        float get_time_scale() const;
};

struct SimulationEvents {
    bool reset = false;
    bool step = false;
};

class SimulationController {
    private:
        SimulationState state;
        SimulationEvents events;

        bool pause_pressed;
        bool step_pressed;
        bool reset_pressed;
        bool half_time_pressed;
        bool double_time_pressed;
        bool reset_time_pressed;

    public:
        SimulationController(bool paused = false, bool step = false);

        void update_state(GLFWwindow* window);
        void update_window_title(GLFWwindow* window);

        const SimulationState& get_state() const;
        const SimulationEvents& get_events() const;
};

#endif