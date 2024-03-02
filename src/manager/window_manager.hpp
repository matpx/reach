#pragma once

#include <manager/manager.hpp>

struct GLFWwindow;

namespace reach {

class WindowManager final : public Manager {
    private:
        GLFWwindow *glfw_window = nullptr;

    public:
        static WindowManager &get();

        WindowManager();
        ~WindowManager();

        bool should_close();
        void finish_frame();
};

} // namespace reach
