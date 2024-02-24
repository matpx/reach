#include "window.hpp"
#include "log.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace reach {

void glfw_error_callback(int error, const char *description) { log_error(description); }

Window::Window() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        log_fatal_and_abort("glfwInit() failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(1200, 800, "Reach", NULL, NULL);
    if (!glfw_window) {
        log_fatal_and_abort("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);
}

Window::~Window() {
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

bool Window::should_close() { return glfwWindowShouldClose(glfw_window); }

void Window::frame() {
    glfwSwapBuffers(glfw_window);
    glfwPollEvents();
}

} // namespace reach
