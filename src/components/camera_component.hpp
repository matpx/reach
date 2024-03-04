#pragma once

#include <ext/matrix_clip_space.hpp>
#include <mat4x4.hpp>

namespace reach {

struct CameraComponent {
        glm::mat4 projection;

        static CameraComponent make(float fovy, float aspect, float z_near, float z_far) {
            return {glm::perspective(fovy, aspect, z_near, z_far)};
        }
};

} // namespace reach
