#pragma once

#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <mat4x4.hpp>
#include <vec3.hpp>

namespace reach {

struct TransformComponent {
        glm::mat4 model;

        glm::vec3 translation;
        glm::quat rotation;
};

} // namespace reach
