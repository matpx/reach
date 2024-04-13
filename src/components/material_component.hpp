#pragma once

#include <nvrhi/nvrhi.h>
#include <string_view>

namespace reach {

struct MaterialComponent {
        std::string_view debug_name = "<unnamed MaterialComponent>";

        nvrhi::GraphicsPipelineHandle graphics_pipeline;
        nvrhi::BindingSetHandle binding_set;
};

} // namespace reach
