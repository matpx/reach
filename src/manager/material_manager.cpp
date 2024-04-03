#include <manager/material_manager.hpp>
#include <utils/conditions.hpp>

#include <unlit.glsl.h>

namespace reach {

static MaterialManager *self = nullptr;

MaterialManager &MaterialManager::get() { return *self; }

MaterialManager::MaterialManager() {
    PRECONDITION(self == nullptr);

    {
        const sg_shader unlit_shader = sg_make_shader(unlit_unlit_shader_desc(sg_query_backend()));
        POSTCONDITION(unlit_shader.id != SG_INVALID_ID);

        sg_pipeline_desc unlit_pipeline_desc = {};
        unlit_pipeline_desc.layout.attrs[ATTR_unlit_vs_position].format = SG_VERTEXFORMAT_FLOAT3;
        unlit_pipeline_desc.layout.attrs[ATTR_unlit_vs_normal].format = SG_VERTEXFORMAT_FLOAT3;
        unlit_pipeline_desc.shader = unlit_shader;
        unlit_pipeline_desc.index_type = SG_INDEXTYPE_UINT32;
        unlit_pipeline_desc.cull_mode = SG_CULLMODE_BACK;
        unlit_pipeline_desc.face_winding = SG_FACEWINDING_CCW;
        unlit_pipeline_desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
        unlit_pipeline_desc.depth.write_enabled = true;
        unlit_pipeline_desc.label = "unlit pipeline";

        unlit_material.pipeline = sg_make_pipeline(unlit_pipeline_desc);
        POSTCONDITION(unlit_material.pipeline.id != SG_INVALID_ID);

        unlit_material.uniform_transform_slot = static_cast<uint8_t>(unlit_unlit_uniformblock_slot(SG_SHADERSTAGE_VS, "transform_params"));
    }

    self = this;
}

MaterialManager::~MaterialManager() {
    PRECONDITION(self != nullptr);
    self = nullptr;
}

} // namespace reach