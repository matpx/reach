#pragma once

#include <manager/manager.hpp>
#include <vec2.hpp>

struct GLFWwindow;

namespace reach {

class WindowManager final : public Manager {
    private:
        GLFWwindow *glfw_window = nullptr;
        glm::ivec2 window_width_height;
        // int32_t sample_count = 1;

    public:
        [[nodiscard]] static WindowManager &get();

        WindowManager(const glm::ivec2 width_height);
        ~WindowManager();

        [[nodiscard]] bool should_close();
        void poll();

        // [[nodiscard]] constexpr int32_t get_sample_count() const { return sample_count; }
        [[nodiscard]] constexpr GLFWwindow *get_glfw_window() const { return glfw_window; };
        [[nodiscard]] constexpr glm::ivec2 get_window_width_height() const { return window_width_height; }
};

} // namespace reach
