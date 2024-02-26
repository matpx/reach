#include "renderer.hpp"
#include "log.hpp"
#include "src/renderer.hpp"

#include "unlit.glsl.h"

namespace reach {

void sokol_log([[maybe_unused]] const char *tag, [[maybe_unused]] uint32_t log_level,
               [[maybe_unused]] uint32_t log_item, const char *message, [[maybe_unused]] uint32_t line_nr,
               [[maybe_unused]] const char *filename, [[maybe_unused]] void *user_data) {
    log_error(message);
}

Renderer::Renderer() {
    sg_setup(sg_desc{.logger = {.func = sokol_log}});
    if (!sg_isvalid()) {
        log_fatal_and_abort("sg_setup() failed");
    }

    const sg_shader unlit_shader = sg_make_shader(unlit_shader_desc(sg_query_backend()));

    sg_pipeline_desc unlit_pipeline_desc = {};
    unlit_pipeline_desc.layout.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3;
    unlit_pipeline_desc.shader = unlit_shader;
    unlit_pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
    unlit_pipeline_desc.cull_mode = SG_CULLMODE_BACK;
    unlit_pipeline_desc.depth = {
        .compare = SG_COMPAREFUNC_LESS_EQUAL,
        .write_enabled = true,
    };
    unlit_pipeline_desc.label = "unlit pipeline";

    unlit_pipeline = sg_make_pipeline(unlit_pipeline_desc);
}

Renderer::~Renderer() { sg_shutdown(); }

} // namespace reach
