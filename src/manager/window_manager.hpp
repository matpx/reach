#pragma once

#include <manager/manager.hpp>
#include <vec2.hpp>

struct GLFWwindow;

namespace reach {

class WindowManager final : public Manager {
    private:
        GLFWwindow *glfw_window = nullptr;
        glm::ivec2 framebuffer_width_height;
        int32_t sample_count = 1;

    public:
        [[nodiscard]] static WindowManager &get();

        WindowManager(const glm::ivec2 width_height);
        ~WindowManager();

        [[nodiscard]] bool should_close();
        void swap();
        void poll();

        [[nodiscard]] constexpr int32_t get_sample_count() const { return sample_count; }
        [[nodiscard]] glm::ivec2 get_framebuffer_width_height() const { return framebuffer_width_height; }
};

} // namespace reach
