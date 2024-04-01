#include <GLFW/glfw3.h>
#include <manager/input_manager.hpp>
#include <utils/conditions.hpp>

namespace reach {

static const std::unordered_map<int32_t, InputAction> key_to_action = {{GLFW_KEY_A, InputAction::LEFT},
                                                                       {GLFW_KEY_D, InputAction::RIGHT},
                                                                       {GLFW_KEY_W, InputAction::UP},
                                                                       {GLFW_KEY_S, InputAction::DOWN},
                                                                       {GLFW_KEY_SPACE, InputAction::JUMP}};

static InputManager *self = nullptr;

InputManager &InputManager::get() { return *self; }

InputManager::InputManager() {
    PRECONDITION(self == nullptr);

    self = this;
}

InputManager::~InputManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
}

void InputManager::reset() {
    just_pressed_keys.clear();
    just_pressed_actions.clear();

    previous_mouse_position = mouse_position;
    mouse_position_delta = {};
}

void InputManager::key_callback(int key, int action) {
    const auto key_action = key_to_action.find(key);

    if (action == GLFW_PRESS) {
        pressed_keys.insert(key);
        just_pressed_keys.insert(key);

        if (key_action != key_to_action.end()) {
            pressed_actions.insert(key_action->second);
            just_pressed_actions.insert(key_action->second);
        }
    } else if (action == GLFW_RELEASE) {
        pressed_keys.erase(key);

        if (key_action != key_to_action.end()) {
            pressed_actions.erase(key_action->second);
        }
    }
}

void InputManager::cursor_position_callback(double x, double y) {
    mouse_position = {x, y};
    mouse_position_delta = mouse_position - previous_mouse_position;
}

} // namespace reach
