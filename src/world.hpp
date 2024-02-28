#pragma once

#include "entt/entt.hpp"
#include "nocopy.hpp"
#include "src/nocopy.hpp"

namespace reach {

class World : public entt::registry, NoCopy {};

} // namespace reach
