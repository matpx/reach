#pragma once

#include <entt/entt.hpp>
#include <utils/nocopy.hpp>

namespace reach {

class World : public entt::registry, NoCopy {
    public:
        static World &current();

        World();
        ~World();
};

} // namespace reach
