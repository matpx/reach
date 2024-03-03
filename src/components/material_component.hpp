#pragma once

#include <sokol_gfx.h>
#include <string_view>

namespace reach {

struct MaterialComponent {
        std::string_view debug_name = "<unnamed MaterialComponent>";

        sg_pipeline pipeline;
        uint8_t uniform_transform_slot;
};

} // namespace reach
