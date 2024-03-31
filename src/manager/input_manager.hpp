#pragma once

#include <manager/manager.hpp>
#include <unordered_set>
#include <vec2.hpp>

struct GLFWwindow;

namespace reach {

enum class InputAction : uint8_t {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3,
    JUMP = 4,
};

class InputManager final : public Manager {
    private:
        std::unordered_set<int> pressed_keys;
        std::unordered_set<int> just_pressed_keys;

        std::unordered_set<InputAction> pressed_actions;
        std::unordered_set<InputAction> just_pressed_actions;

        glm::vec2 mouse_position = {};

    public:
        [[nodiscard]] static InputManager &get();

        InputManager();
        ~InputManager();

        void reset();

        void key_callback(int key, int action);
        void cursor_position_callback(double x, double y);

        [[nodiscard]] inline bool is_key_pressed(const int key) const { return pressed_keys.contains(key); }
        [[nodiscard]] inline bool is_key_just_pressed(const int key) const { return just_pressed_keys.contains(key); }

        [[nodiscard]] inline bool is_action_pressed(const InputAction action) const { return pressed_actions.contains(action); }
        [[nodiscard]] inline bool is_action_just_pressed(const InputAction action) const { return just_pressed_actions.contains(action); }

        [[nodiscard]] inline const glm::vec2 &get_mouse_position() const { return mouse_position; }
};

} // namespace reach