#include "window.hpp"
#include "fatal.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace reach {

void glfw_error_callback(int error, const char *description) { log_error(description); }

Window::Window() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        fatal("glfwInit() failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfw_window = glfwCreateWindow(1200, 800, "Reach", NULL, NULL);
    if (!glfw_window) {
        fatal("glfwInit() failed");
    }

    glfwMakeContextCurrent(glfw_window);
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
