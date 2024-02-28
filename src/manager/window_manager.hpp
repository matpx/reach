#pragma once

#include "nocopy.hpp"

struct GLFWwindow;

namespace reach {

class WindowManager : NoCopy {
    private:
        GLFWwindow *glfw_window = nullptr;

    public:
        WindowManager();
        ~WindowManager();

        bool should_close();
        void finish_frame();
};

} // namespace reach
