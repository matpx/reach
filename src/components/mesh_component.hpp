#pragma once

#include <data/mesh_data.hpp>
#include <memory>
#include <sokol_gfx.h>
#include <stdint.h>
#include <string_view>

namespace reach {

struct MeshComponent {
        std::string_view _debug_name;

        std::shared_ptr<MeshData> mesh_data;

        sg_buffer vertex_buffer;
        sg_buffer index_buffer;

        uint32_t index_count;

        constexpr std::string_view get_debug_name() {
            return _debug_name.empty() ? "<unnamed MeshComponent>" : _debug_name;
        }
};

} // namespace reach
