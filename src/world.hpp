#pragma once

#include "entt/entt.hpp"
#include "nocopy.hpp"
#include "src/nocopy.hpp"

namespace reach {

class World : entt::registry, NoCopy {};

} // namespace reach
