#pragma once

#include <data/mesh_data.hpp>
#include <memory>
#include <string_view>

namespace reach {

struct MeshComponent {
        std::string_view debug_name = "<unnamed MeshComponent>";

        std::shared_ptr<MeshData> mesh_data;

        uint32_t index_count;
        uint32_t index_offset;
};

} // namespace reach
