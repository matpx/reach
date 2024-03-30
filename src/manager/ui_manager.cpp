#include <components/camera_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/ui_manager.hpp>
#include <utils/conditions.hpp>
#include <world.hpp>

#include <immediate.glsl.h>

namespace reach {

static UiManager *self = nullptr;

UiManager &UiManager::get() { return *self; }

UiManager::UiManager() {
    PRECONDITION(self == nullptr);

    {
        const sg_shader immediate_shader = sg_make_shader(immediate_immediate_shader_desc(sg_query_backend()));
        POSTCONDITION(immediate_shader.id != SG_INVALID_ID);

        sg_pipeline_desc immediate_pipeline_desc = {};
        immediate_pipeline_desc.layout.attrs[ATTR_immediate_vs_position].format = SG_VERTEXFORMAT_FLOAT3;
        immediate_pipeline_desc.shader = immediate_shader;
        immediate_pipeline_desc.index_type = SG_INDEXTYPE_NONE;
        immediate_pipeline_desc.cull_mode = SG_CULLMODE_NONE;
        immediate_pipeline_desc.depth.write_enabled = false;
        immediate_pipeline_desc.label = "immediate pipeline";

        immediate_material.pipeline = sg_make_pipeline(immediate_pipeline_desc);
        POSTCONDITION(immediate_material.pipeline.id != SG_INVALID_ID);

        immediate_material.uniform_transform_slot =
            static_cast<uint8_t>(immediate_immediate_uniformblock_slot(SG_SHADERSTAGE_VS, "transform_params"));
    }

    self = this;
}

UiManager::~UiManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
}

void UiManager::submit_to_device() {
    if (vertex_data.size() == 0) {
        return;
    }

    const auto &world = World::current();

    DeviceManager::get().draw_immediate(world.get<CameraComponent>(world.current_camera).ui_proj, vertex_data, immediate_material);

    vertex_data.resize(0);
}

} // namespace reach
