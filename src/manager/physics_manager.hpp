#pragma once

#include <manager/manager.hpp>
#include <memory>

namespace JPH {

class TempAllocatorImpl;
class JobSystemThreadPool;
class PhysicsSystem;

} // namespace JPH

namespace reach {

class PhysicsManager final : public Manager {
    private:
        std::unique_ptr<JPH::TempAllocatorImpl> temp_allocator;
        std::unique_ptr<JPH::JobSystemThreadPool> job_system;
        std::unique_ptr<JPH::PhysicsSystem> physics_system;

    public:
        [[nodiscard]] static PhysicsManager &get();

        PhysicsManager();
        ~PhysicsManager();

        void update(const float dt);
};

} // namespace reach
