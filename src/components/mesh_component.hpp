#pragma once

#include <data/mesh_data.hpp>
#include <memory>
#include <sokol_gfx.h>
#include <string_view>

namespace reach {

struct MeshComponent {
        std::string_view debug_name = "<unnamed MeshComponent>";

        bool visible = true;

        std::shared_ptr<MeshData> mesh_data;

        sg_buffer vertex_buffer;
        sg_buffer index_buffer;

        uint32_t index_count;
        uint32_t index_offset;
};

} // namespace reach
