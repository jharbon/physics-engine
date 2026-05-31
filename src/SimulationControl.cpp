#include <SimulationControl.hpp>

SimulationState::SimulationState(bool paused, bool step)
        : paused{paused}, step{step}, reset{false} {}

void SimulationState::toggle_pause() {
    this->paused = !this->paused;
}

void SimulationState::toggle_step() {
    this->step = !this->step;
}

void SimulationState::toggle_reset() {
    this->reset = !this->reset;
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

SimulationController::SimulationController(bool paused, bool step) 
        : state{paused, step}, pause_pressed{false}, step_pressed{false}, reset_pressed{false} {}

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
}

const SimulationState& SimulationController::get_state() const {
    return this->state;
}