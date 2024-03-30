#pragma once

#include <ext/matrix_clip_space.hpp>
#include <mat4x4.hpp>

namespace reach {

struct CameraComponent {
        glm::mat4 projection;
        glm::mat4 ortho;

        static CameraComponent make(float fovy, const glm::ivec2 width_height, float z_near, float z_far) {
            return {
                .projection =
                    glm::perspective(fovy, static_cast<float>(width_height.x) / static_cast<float>(width_height.y), z_near, z_far),
                .ortho = glm::orthoRH(0.0f, static_cast<float>(width_height.x), static_cast<float>(width_height.y), 0.0f, -1.0f, 1.0f),
            };
        }
};

} // namespace reach
