#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/device_manager.hpp>
#include <manager/window_manager.hpp>
#include <utils/conditions.hpp>

namespace reach {

static DeviceManager *self = nullptr;

DeviceManager &DeviceManager::get() { return *self; }

DeviceManager::DeviceManager() {
    PRECONDITION(self == nullptr);

    self = this;
}

DeviceManager::~DeviceManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
}

void DeviceManager::upload_meshdata(MeshData &mesh_data) {
    LOG_DEBUG("updating mesh_data: {}", mesh_data.debug_name);

    // mesh_data.vertex_buffer =
    //     sg_make_buffer(sg_buffer_desc{.data = {
    //                                       .ptr = mesh_data.vertex_data.data(),
    //                                       .size = mesh_data.vertex_data.size() * sizeof(decltype(mesh_data.vertex_data)::value_type),
    //                                   }});

    // mesh_data.index_buffer =
    //     sg_make_buffer(sg_buffer_desc{.type = SG_BUFFERTYPE_INDEXBUFFER,
    //                                   .data = {
    //                                       .ptr = mesh_data.index_data.data(),
    //                                       .size = mesh_data.index_data.size() * sizeof(decltype(mesh_data.index_data)::value_type),
    //                                   }});
}

void DeviceManager::begin_main_pass() {
    PRECONDITION(pass_is_active == false);

    // const glm::ivec2 width_height = WindowManager::get().get_framebuffer_width_height();

    pass_is_active = true;
}

void DeviceManager::draw_mesh(const glm::mat4 &model_view_projection, const MaterialComponent &material_component,
                              const MeshComponent &mesh_component) {
    PRECONDITION(pass_is_active == true);
}

void DeviceManager::draw_immediate(const glm::mat4 &projection, const std::span<const Vertex2D> vertex_data,
                                   const MaterialComponent &material_component) {
    // const std::size_t required_byte_size = vertex_data.size() * sizeof(decltype(vertex_data)::value_type);

    // if (immediate_buffer_desc.size < required_byte_size) {
    //     LOG_DEBUG("destroy immediate buffer because immediate_buffer_desc.size < required_byte_size");

    //     sg_destroy_buffer(immediate_buffer);

    //     immediate_buffer = {};
    //     immediate_buffer_desc = {};
    // }

    // if (immediate_buffer.id == SG_INVALID_ID) {
    //     LOG_DEBUG("create immediate buffer");

    //     immediate_buffer_desc.data.size = required_byte_size;
    //     immediate_buffer_desc.usage = SG_USAGE_STREAM;

    //     immediate_buffer = sg_make_buffer(immediate_buffer_desc);
    //     immediate_buffer_desc.size = required_byte_size;
    // }

    // sg_update_buffer(immediate_buffer, sg_range{.ptr = vertex_data.data(), .size = required_byte_size});

    // sg_apply_pipeline(material_component.pipeline);
    // sg_apply_uniforms(SG_SHADERSTAGE_VS, material_component.uniform_transform_slot, SG_RANGE(projection));
    // sg_apply_bindings(sg_bindings{
    //     .vertex_buffers = {immediate_buffer},
    // });

    // sg_draw(0, static_cast<int32_t>(vertex_data.size()), 1);
}

void DeviceManager::finish_main_pass() {
    PRECONDITION(pass_is_active == true);

    pass_is_active = false;

    // sg_end_pass();
    // sg_commit();

    // collect_gargabe();
}

} // namespace reach
