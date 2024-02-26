#include "world.hpp"
#include "ext/matrix_transform.hpp"
#include "src/transform.hpp"
#include "src/world.hpp"
#include "transform.hpp"

namespace reach {

void World::update_transforms() {
    for (auto [entity, transform] : view<Transform>().each()) {
        transform.model =
            glm::translate(glm::identity<glm::mat4>(), transform.translation) * glm::toMat4(transform.rotation);
    }
}

} // namespace reach