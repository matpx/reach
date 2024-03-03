#pragma once

#include <manager/manager.hpp>
#include <utility>
#include <vec2.hpp>

struct GLFWwindow;

namespace reach {

class WindowManager final : public Manager {
    private:
        GLFWwindow *glfw_window = nullptr;
        glm::ivec2 width_height = {1200, 800};
        int32_t sample_count = 1;

    public:
        [[nodiscard]] static WindowManager &get();

        WindowManager();
        ~WindowManager();

        [[nodiscard]] bool should_close();
        void finish_frame();

        [[nodiscard]] constexpr glm::ivec2 get_width_height() const { return width_height; }
        [[nodiscard]] constexpr int32_t get_sample_count() const { return sample_count; }
};

} // namespace reach
