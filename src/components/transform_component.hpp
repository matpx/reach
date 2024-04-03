#pragma once

#include <entt/entity/entity.hpp>
#include <gtc/quaternion.hpp>
#include <mat4x4.hpp>
#include <vec3.hpp>

namespace reach {

struct TransformParent {
        entt::entity id;
};

struct TransformComponent {
        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 world = glm::identity<glm::mat4>();

        glm::vec3 translation = glm::zero<glm::vec3>();
        glm::quat rotation = glm::identity<glm::quat>();
};

} // namespace reach
