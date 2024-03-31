#pragma once

#include <manager/manager.hpp>

namespace reach {

class InputManager final : public Manager {
    private:
    public:
        [[nodiscard]] static InputManager &get();

        InputManager();
        ~InputManager();
};

} // namespace reach