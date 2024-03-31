#include <manager/input_manager.hpp>
#include <utils/conditions.hpp>

namespace reach {

static InputManager *self = nullptr;

InputManager &InputManager::get() { return *self; }

InputManager::InputManager() {
    PRECONDITION(self == nullptr);

    self = this;
}

InputManager::~InputManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
}

} // namespace reach
