#pragma once

#include <manager/manager.hpp>
#include <memory>

namespace JPH {
class PhysicsSystem;
}

namespace reach {

class PhysicsManager final : public Manager {
    private:
        std::unique_ptr<JPH::PhysicsSystem> physics_system;

    public:
        [[nodiscard]] static PhysicsManager &get();

        PhysicsManager();
        ~PhysicsManager();
};

} // namespace reach
