#include "data/mesh_data.hpp"
#include "utils/log.hpp"
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

DeviceManager::DeviceManager() {
    sg_setup(sg_desc{.logger = {.func = sokol_log}});
    if (!sg_isvalid()) {
        PANIC("sg_setup() failed");
    }
}

DeviceManager::~DeviceManager() { sg_shutdown(); }

void DeviceManager::update_mesh(MeshComponent &mesh_component) {
    if (!mesh_component.mesh_data) {
        LOG_ERROR("cannot update MeshComponent with empty mesh_data");
    }

    // unload old data if mesh is being updated
    unload_mesh(mesh_component);

    LOG_DEBUG("uploading mesh: {}", mesh_component.debug_name);

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

} // namespace reach
