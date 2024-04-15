#include <components/camera_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/ui_manager.hpp>
#include <utils/conditions.hpp>
#include <world.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "immediate_main_ps.dxbc.h"
#include "immediate_main_vs.dxbc.h"

namespace reach {

static UiManager *self = nullptr;

UiManager &UiManager::get() { return *self; }

UiManager::UiManager() {
    PRECONDITION(self == nullptr);

    {
        auto &nvrhi_device = DeviceManager::get().get_nvrhi_device();
        auto &framebuffer = DeviceManager::get().get_nvrhi_framebuffer();

        LOG_INFO("init ui material pipeline");

        nvrhi::ShaderHandle vertex_shader = nvrhi_device->createShader(nvrhi::ShaderDesc(nvrhi::ShaderType::Vertex),
                                                                       g_immediate_main_vs_dxbc, sizeof(g_immediate_main_vs_dxbc));

        nvrhi::ShaderHandle pixel_shader = nvrhi_device->createShader(nvrhi::ShaderDesc(nvrhi::ShaderType::Pixel), g_immediate_main_ps_dxbc,
                                                                      sizeof(g_immediate_main_ps_dxbc));

        nvrhi::VertexAttributeDesc attributes[] = {
            nvrhi::VertexAttributeDesc()
                .setName("POSITION")
                .setFormat(nvrhi::Format::RG32_FLOAT)
                .setOffset(offsetof(Vertex2D, position))
                .setElementStride(sizeof(Vertex2D)),
            nvrhi::VertexAttributeDesc()
                .setName("COLOR")
                .setFormat(nvrhi::Format::RGBA32_FLOAT)
                .setOffset(offsetof(Vertex2D, color))
                .setElementStride(sizeof(Vertex2D)),
        };

        nvrhi::InputLayoutHandle input_layout = nvrhi_device->createInputLayout(attributes, uint32_t(std::size(attributes)), vertex_shader);

        auto binding_layout_desc = nvrhi::BindingLayoutDesc()
                                       .setVisibility(nvrhi::ShaderType::Vertex)
                                       .addItem(nvrhi::BindingLayoutItem::VolatileConstantBuffer(0));

        nvrhi::BindingLayoutHandle binding_layout = nvrhi_device->createBindingLayout(binding_layout_desc);

        const auto render_state = nvrhi::RenderState().setRasterState(nvrhi::RasterState().setFrontCounterClockwise(true));
        const auto pipeline_desc = nvrhi::GraphicsPipelineDesc()
                                       .setInputLayout(input_layout)
                                       .setVertexShader(vertex_shader)
                                       .setPixelShader(pixel_shader)
                                       .addBindingLayout(binding_layout)
                                       .setRenderState(render_state);

        immediate_material.graphics_pipeline = nvrhi_device->createGraphicsPipeline(pipeline_desc, framebuffer);

        const auto binding_set_desc =
            nvrhi::BindingSetDesc().addItem(nvrhi::BindingSetItem::ConstantBuffer(0, DeviceManager::get().get_transform_constant_buffer()));
        immediate_material.binding_set = nvrhi_device->createBindingSet(binding_set_desc, binding_layout);
    }

    self = this;
}

UiManager::~UiManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
}

void UiManager::submit_to_device() {
    if (immediate_data.size() == 0) {
        return;
    }

    const auto &world = World::current();

    if (world.current_camera != entt::null) {
        DeviceManager::get().draw_immediate(world.get<CameraComponent>(world.current_camera).ui_proj, immediate_data, immediate_material);
    }

    immediate_data.resize(0);
}

} // namespace reach
