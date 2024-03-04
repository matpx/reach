#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/window_manager.hpp>
#include <sokol_gfx.h>
#include <utils/conditions.hpp>
#include <utils/panic.hpp>

namespace reach {

void sokol_log([[maybe_unused]] const char *tag, [[maybe_unused]] uint32_t log_level,
               [[maybe_unused]] uint32_t log_item, const char *message, [[maybe_unused]] uint32_t line_nr,
               [[maybe_unused]] const char *filename, [[maybe_unused]] void *user_data) {
    LOG_ERROR(message);
}

static DeviceManager *self = nullptr;

DeviceManager &DeviceManager::get() { return *self; }

DeviceManager::DeviceManager() {
    PRECONDITION(self == nullptr);

    const sg_environment environment = {.defaults = {
                                            .color_format = SG_PIXELFORMAT_RGBA8,
                                            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
                                            .sample_count = WindowManager::get().get_sample_count(),
                                        }};

    sg_setup(sg_desc{.logger = {.func = sokol_log}, .environment = environment});

    POSTCONDITION(sg_isvalid());

    self = this;
}

DeviceManager::~DeviceManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
    sg_shutdown();
}

void DeviceManager::update_mesh(MeshComponent &mesh_component) {
    if (!mesh_component.mesh_data) {
        LOG_ERROR("cannot update MeshComponent with empty mesh_data");
        return;
    }

    if (mesh_component.vertex_buffer.id != SG_INVALID_ID || mesh_component.index_buffer.id != SG_INVALID_ID) {
        unload_mesh(mesh_component);
    }

    LOG_DEBUG("updating mesh: {}", mesh_component.debug_name);

    const auto &mesh_data = mesh_component.mesh_data;

    mesh_component.vertex_buffer = sg_make_buffer(
        sg_buffer_desc{.data = {
                           .ptr = mesh_data->vertex_data.data(),
                           .size = mesh_data->vertex_data.size() * sizeof(decltype(mesh_data->vertex_data)::value_type),
                       }});

    mesh_component.index_buffer = sg_make_buffer(
        sg_buffer_desc{.type = SG_BUFFERTYPE_INDEXBUFFER,
                       .data = {
                           .ptr = mesh_data->index_data.data(),
                           .size = mesh_data->index_data.size() * sizeof(decltype(mesh_data->index_data)::value_type),
                       }});

    mesh_component.index_count = static_cast<uint32_t>(mesh_data->index_data.size());
}

void DeviceManager::unload_mesh(MeshComponent &mesh_component) {
    LOG_DEBUG("unloading mesh: {}", mesh_component.debug_name);

    if (mesh_component.vertex_buffer.id != SG_INVALID_ID) {
        sg_destroy_buffer(mesh_component.vertex_buffer);
        mesh_component.vertex_buffer = {};
    }

    if (mesh_component.index_buffer.id != SG_INVALID_ID) {
        sg_destroy_buffer(mesh_component.index_buffer);
        mesh_component.index_buffer = {};
    }
}

void DeviceManager::begin_main_pass() {
    PRECONDITION(pass_is_active == false);

    const glm::ivec2 width_height = WindowManager::get().get_width_height();

    const sg_swapchain swapchain = {.width = width_height.x,
                                    .height = width_height.y,
                                    .sample_count = WindowManager::get().get_sample_count(),
                                    .color_format = SG_PIXELFORMAT_RGBA8,
                                    .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
                                    .gl = {
                                        .framebuffer = 0,
                                    }};

    sg_pass_action pass_action = {};

    sg_begin_pass(sg_pass{.action = pass_action, .swapchain = swapchain});

    pass_is_active = true;
}

void DeviceManager::draw_mesh(const TransformComponent &transform_component,
                              const MaterialComponent &material_component, const MeshComponent &mesh_component) {
    PRECONDITION(pass_is_active);

    const glm::mat4 mode_view_projection = transform_component.model;

    sg_apply_pipeline(material_component.pipeline);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, material_component.uniform_transform_slot, SG_RANGE(mode_view_projection));
    sg_apply_bindings(sg_bindings{
        .vertex_buffers = {mesh_component.vertex_buffer},
        .index_buffer = mesh_component.index_buffer,
    });
    sg_draw(0, mesh_component.index_count, 1);
}

void DeviceManager::finish_main_pass() {
    PRECONDITION(pass_is_active == true);

    pass_is_active = false;

    sg_end_pass();
    sg_commit();
}

} // namespace reach
