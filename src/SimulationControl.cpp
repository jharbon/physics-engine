#include <SimulationControl.hpp>

#include <fmt/core.h>

#include <stdexcept>

SimulationState::SimulationState(bool paused, bool step, float time_scale, float min_time_scale, float max_time_scale)
        : paused{paused}, step{step}, reset{false}, min_time_scale{min_time_scale}, max_time_scale{max_time_scale} {
    if (time_scale < min_time_scale || time_scale > max_time_scale) {
        std::string msg = fmt::format(
            "Provided time_scale with value {} to SimulationState constructor but this is outside the configured boundaries of [{}, {}]",
            time_scale,
            min_time_scale,
            max_time_scale
        );
        
        throw std::invalid_argument(msg);
    }

    this->time_scale = time_scale;
}

void SimulationState::toggle_pause() {
    this->paused = !this->paused;
}

void SimulationState::toggle_step() {
    this->step = !this->step;
}

void SimulationState::toggle_reset() {
    this->reset = !this->reset;
}

void SimulationState::half_time_scale() {
    float new_scale = this->time_scale * 0.5;

    if (new_scale >= this->min_time_scale) {
        this->time_scale = new_scale;
    }
}

void SimulationState::double_time_scale() {
    float new_scale = this->time_scale * 2;

    if (new_scale <= this->max_time_scale) {
        this->time_scale = new_scale;
    }
}

void SimulationState::reset_time_scale() {
    this->time_scale = sim_control::DEFAULT_TIME_SCALE;
}

bool SimulationState::is_paused() const {
    return this->paused;
}

bool SimulationState::is_step() const {
    return this->step;
}

bool SimulationState::is_reset() const {
    return this->reset;
}

float SimulationState::get_time_scale() const {
    return this->time_scale;
}

SimulationController::SimulationController(bool paused, bool step) 
        : state{paused, step}, pause_pressed{false}, step_pressed{false}, reset_pressed{false},
          half_time_pressed{false}, double_time_pressed{false}, reset_time_pressed{false} {}

void SimulationController::update_state(GLFWwindow* window) {
    if (glfwGetKey(window, key_bindings::PAUSE) == GLFW_PRESS) {
        if (!this->pause_pressed) {
            this->pause_pressed = true;
            this->state.toggle_pause();
        }
    }
    else {
        this->pause_pressed = false;
    }

    if (glfwGetKey(window, key_bindings::STEP) == GLFW_PRESS) {
        if (!this->step_pressed) {
            this->step_pressed = true;
            this->state.toggle_step();
            if (this->state.is_paused()) {
                // Un-pause to enable simulation to run for single step
                this->state.toggle_pause();
            }
        }
    }
    else {
        this->step_pressed = false;
        if (this->state.is_step() && !this->state.is_paused()) {
            // Single step has completed; turn step off and pause
            this->state.toggle_step();
            this->state.toggle_pause();
        }

    }

    if (glfwGetKey(window, key_bindings::RESET) == GLFW_PRESS) {
        if (!this->reset_pressed) {
            this->reset_pressed = true;
            this->state.toggle_reset();
        }
    }
    else {
        this->reset_pressed = false;
        if (this->state.is_reset()) {
            this->state.toggle_reset();
        }
    }

    if (glfwGetKey(window, key_bindings::HALF_TIME_SCALE) == GLFW_PRESS) {
        if (!this->half_time_pressed && !this->double_time_pressed && !this->reset_time_pressed) {
            this->half_time_pressed = true;
            this->state.half_time_scale();
            this->update_window_title(window);
        }
    }
    else {
        this->half_time_pressed = false;
    }

    if (glfwGetKey(window, key_bindings::DOUBLE_TIME_SCALE) == GLFW_PRESS) {
        if (!this->half_time_pressed && !this->double_time_pressed && !this->reset_time_pressed) {
            this->double_time_pressed = true;
            this->state.double_time_scale();
            this->update_window_title(window);
        }
    }
    else {
        this->double_time_pressed = false;
    }

    if (glfwGetKey(window, key_bindings::RESET_TIME_SCALE) == GLFW_PRESS) {
        if (!this->half_time_pressed && !this->double_time_pressed && !this->reset_time_pressed) {
            this->reset_time_pressed = true;
            this->state.reset_time_scale();
            this->update_window_title(window);
        }
    }
    else {
        this->reset_time_pressed = false;
    }
}

void SimulationController::update_window_title(GLFWwindow* window) {
    std::string new_title = fmt::format(
        "Physics Engine - time_scale={}x",
        this->state.get_time_scale()
    );

    glfwSetWindowTitle(window, new_title.c_str());
}

const SimulationState& SimulationController::get_state() const {
    return this->state;
}