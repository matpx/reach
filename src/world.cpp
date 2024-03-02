#include <world.hpp>

namespace reach {

static World *self = nullptr;

World &World::current() { return *self; }

World::World() { self = this; }

World::~World() { self = nullptr; }

} // namespace reach
