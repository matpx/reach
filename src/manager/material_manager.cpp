#include <manager/material_manager.hpp>
#include <sokol_gfx.h>

#include <unlit.glsl.h>

namespace reach {

static MaterialManager *self = nullptr;

MaterialManager &MaterialManager::get() { return *self; }

MaterialManager::MaterialManager() {
    const sg_shader unlit_shader = sg_make_shader(unlit_shader_desc(sg_query_backend()));

    sg_pipeline_desc unlit_pipeline_desc = {};
    unlit_pipeline_desc.layout.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3;
    unlit_pipeline_desc.shader = unlit_shader;
    unlit_pipeline_desc.index_type = SG_INDEXTYPE_UINT32;
    unlit_pipeline_desc.cull_mode = SG_CULLMODE_BACK;
    unlit_pipeline_desc.depth = {
        .compare = SG_COMPAREFUNC_LESS_EQUAL,
        .write_enabled = true,
    };
    unlit_pipeline_desc.label = "unlit pipeline";

    unlit_material.pipeline = sg_make_pipeline(unlit_pipeline_desc);

    self = this;
}

MaterialManager::~MaterialManager() { self = nullptr; }

} // namespace reach