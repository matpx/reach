#pragma once

#include <entt/entt.hpp>
#include <utils/nocopy.hpp>

namespace reach {

class World : public entt::registry, NoCopy {
    public:
        [[nodiscard]] static World &current();

        World();
        ~World();

        entt::entity current_camera = entt::null;
};

} // namespace reach
