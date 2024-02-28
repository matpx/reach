#include "world.hpp"
#include "components/transform_component.hpp"
#include "ext/matrix_transform.hpp"
#include "world.hpp"


namespace reach {

void World::update_transforms() {
    for (auto [entity, transform] : view<TransformComponent>().each()) {
        transform.model =
            glm::translate(glm::identity<glm::mat4>(), transform.translation) * glm::toMat4(transform.rotation);
    }
}

} // namespace reach