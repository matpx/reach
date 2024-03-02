#pragma once

#include <data/mesh_data.hpp>
#include <memory>
#include <sokol_gfx.h>
#include <string_view>

namespace reach {

struct MeshComponent {
        sg_buffer vertex_buffer;
        sg_buffer index_buffer;

        std::shared_ptr<MeshData> mesh_data;

        std::string_view debug_name;
};

} // namespace reach
