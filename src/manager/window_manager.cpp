#include <manager/window_manager.hpp>
#include <utils/panic.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace reach {

void glfw_error_callback([[maybe_unused]] int error, const char *description) { LOG_ERROR(description); }

WindowManager::WindowManager() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        PANIC("glfwInit() failed");
    }

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(1200, 800, "Reach", NULL, NULL);
    if (!glfw_window) {
        PANIC("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);
}

WindowManager::~WindowManager() {
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

bool WindowManager::should_close() {
    return glfwWindowShouldClose(glfw_window) || glfwGetKey(glfw_window, GLFW_KEY_ESCAPE);
}

void WindowManager::finish_frame() {
    glfwSwapBuffers(glfw_window);
    glfwPollEvents();
}

} // namespace reach
