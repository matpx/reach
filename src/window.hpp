#pragma once

#include "nocopy.hpp"

struct GLFWwindow;

namespace reach {

class Window : NoCopy {
    private:
        GLFWwindow *glfw_window = nullptr;

    public:
        Window();
        ~Window();

        bool should_close();
        void finish_frame();
};

} // namespace reach
