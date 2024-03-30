#pragma once

#include <ext/matrix_clip_space.hpp>
#include <mat4x4.hpp>

namespace reach {

struct CameraComponent {
        glm::mat4 perspective_proj;
        glm::mat4 ui_proj;

        static CameraComponent make(float fovy, const glm::ivec2 width_height, float z_near, float z_far) {
            return {
                .perspective_proj =
                    glm::perspective(fovy, static_cast<float>(width_height.x) / static_cast<float>(width_height.y), z_near, z_far),
                .ui_proj = glm::orthoRH(0.0f, static_cast<float>(width_height.x), static_cast<float>(width_height.y), 0.0f, -1.0f, 1.0f),
            };
        }
};

} // namespace reach
