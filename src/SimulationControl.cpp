#include <SimulationControl.hpp>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

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

void SimulationState::half_time_scale() {
    float old_scale = this->time_scale;
    float new_scale = this->time_scale * 0.5;
    if (new_scale >= this->min_time_scale) {
        this->time_scale = new_scale;
        spdlog::debug("Sim time scale 0.5x: {} -> {}", old_scale, new_scale);
    }
}

void SimulationState::double_time_scale() {
    float old_scale = this->time_scale;
    float new_scale = this->time_scale * 2;
    if (new_scale <= this->max_time_scale) {
        this->time_scale = new_scale;
        spdlog::debug("Sim time scale 2x: {} -> {}", old_scale, new_scale);
    }
}

void SimulationState::reset_time_scale() {
    float old_scale = this->time_scale;
    this->time_scale = sim_control::DEFAULT_TIME_SCALE;
    spdlog::debug("Sim time scale reset: {} -> {}", old_scale, this->time_scale);
}

bool SimulationState::is_paused() const {
    return this->paused;
}

float SimulationState::get_time_scale() const {
    return this->time_scale;
}

SimulationController::SimulationController(bool paused, bool step) 
        : state{paused, step}, pause_pressed{false}, step_pressed{false}, reset_pressed{false},
          half_time_pressed{false}, double_time_pressed{false}, reset_time_pressed{false} {}

void SimulationController::update(GLFWwindow* window) {
    if (glfwGetKey(window, key_bindings::PAUSE) == GLFW_PRESS) {
        if (!this->pause_pressed) {
            this->pause_pressed = true;
            spdlog::debug("Sim pause pressed");
            this->state.toggle_pause();
        }
    }
    else {
        this->pause_pressed = false;
    }

    if (glfwGetKey(window, key_bindings::STEP) == GLFW_PRESS) {
        if (!this->step_pressed) {
            this->step_pressed = true;
            spdlog::debug("Sim step pressed");
            this->events.step = true;
            if (this->state.is_paused()) {
                // Un-pause to enable simulation to run for single step
                this->state.toggle_pause();
            }
        }
    }
    else {
        this->step_pressed = false;
        if (this->events.step && !this->state.is_paused()) {
            // Single step has completed; turn step off and pause
            this->events.step = false;
            this->state.toggle_pause();
        }

    }

    // Set event to false here to ensure single occurrence for a given key press
    this->events.reset_sim = false;
    if (glfwGetKey(window, key_bindings::RESET) == GLFW_PRESS) {
        if (!this->reset_pressed) {
            this->reset_pressed = true;
            spdlog::debug("Sim reset pressed");
            this->events.reset_sim = true;
        }
    }
    else {
        this->reset_pressed = false;
    }

    if (glfwGetKey(window, key_bindings::HALF_TIME_SCALE) == GLFW_PRESS) {
        if (!this->half_time_pressed && !this->double_time_pressed && !this->reset_time_pressed) {
            this->half_time_pressed = true;
            spdlog::debug("Sim 0.5x time scale pressed");
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
            spdlog::debug("Sim 2x time scale pressed");
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
            spdlog::debug("Sim reset time scale pressed");
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

const SimulationEvents& SimulationController::get_events() const {
    return this->events;
}