#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <iterator>
#include <manager/device_manager.hpp>
#include <sokol_gfx.h>
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
    const sg_environment environment = {.defaults = {
                                            .color_format = SG_PIXELFORMAT_RGBA8,
                                            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
                                            .sample_count = 1,
                                        }};

    sg_setup(sg_desc{.logger = {.func = sokol_log}, .environment = environment});

    if (!sg_isvalid()) {
        PANIC("sg_setup() failed");
    }

    self = this;
}

DeviceManager::~DeviceManager() {
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

void DeviceManager::begin_frame() {
    sg_swapchain swapchain = {.width = 1200,
                              .height = 800,
                              .sample_count = 1,
                              .color_format = SG_PIXELFORMAT_RGBA8,
                              .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
                              .gl = {
                                  // we just assume here that the GL framebuffer is always 0
                                  .framebuffer = 0,
                              }};

    sg_pass_action pass_action = {};

    sg_begin_pass(sg_pass{.action = pass_action, .swapchain = swapchain});
}

void DeviceManager::draw_mesh(const MaterialComponent &material, const MeshComponent &mesh_component) {
    sg_apply_pipeline(material.pipeline);
    sg_apply_bindings(sg_bindings{
        .vertex_buffers = {mesh_component.vertex_buffer},
        .index_buffer = mesh_component.index_buffer,
    });
    sg_draw(0, mesh_component.index_count, 1);
}

void DeviceManager::finish_frame() {
    sg_end_pass();
    sg_commit();
}

} // namespace reach
