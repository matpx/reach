#include <GLFW/glfw3.h>
#include <manager/input_manager.hpp>
#include <manager/window_manager.hpp>
#include <utils/conditions.hpp>

namespace reach {

static void glfw_error_callback([[maybe_unused]] int error, const char *description) { LOG_ERROR(description); }

static void key_callback(GLFWwindow * /*window*/, int key, int /*scancode*/, int action, int /*mods*/) {
    InputManager::get().key_callback(key, action);
}

static void cursor_position_callback(GLFWwindow * /*window*/, double xpos, double ypos) {
    InputManager::get().cursor_position_callback(xpos, ypos);
}

static WindowManager *self = nullptr;

WindowManager &WindowManager::get() { return *self; }

WindowManager::WindowManager(const glm::ivec2 width_height) {
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

    glfwGetFramebufferSize(glfw_window, &framebuffer_width_height.x, &framebuffer_width_height.y);

    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);

    {
        double xpos, ypos;
        glfwGetCursorPos(glfw_window, &xpos, &ypos);
        cursor_position_callback(glfw_window, xpos, ypos);
    }

    self = this;
}

WindowManager::~WindowManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;

    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

bool WindowManager::should_close() { return glfwWindowShouldClose(glfw_window) || glfwGetKey(glfw_window, GLFW_KEY_ESCAPE); }

void WindowManager::swap() { glfwSwapBuffers(glfw_window); }

void WindowManager::poll() {
    InputManager::get().reset();
    glfwPollEvents();
}

} // namespace reach
