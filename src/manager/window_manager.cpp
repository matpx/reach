#include <manager/window_manager.hpp>
#include <utils/conditions.hpp>
#include <utils/panic.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace reach {

void glfw_error_callback([[maybe_unused]] int error, const char *description) { LOG_ERROR(description); }

static WindowManager *self = nullptr;

WindowManager &WindowManager::get() { return *self; }

WindowManager::WindowManager() {
    PRECONDITION(self == nullptr);

    glfwSetErrorCallback(glfw_error_callback);

    const int32_t glfw_init_result = glfwInit();
    POSTCONDITION(glfw_init_result != 0);

    glfwWindowHint(GLFW_SAMPLES, sample_count);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(width_height.x, width_height.y, "Reach", NULL, NULL);
    POSTCONDITION(glfw_window != nullptr);

    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);

    self = this;
}

WindowManager::~WindowManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;

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
