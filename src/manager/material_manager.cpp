#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <nvrhi/nvrhi.h>
#include <utils/conditions.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "unlit_main_ps.dxbc.h"
#include "unlit_main_vs.dxbc.h"

namespace reach {

static MaterialManager *self = nullptr;

MaterialManager &MaterialManager::get() { return *self; }

MaterialManager::MaterialManager() {
    PRECONDITION(self == nullptr);

    {
        auto &nvrhi_device = DeviceManager::get().get_nvrhi_device();
        auto &framebuffer = DeviceManager::get().get_nvrhi_framebuffer();

        nvrhi::ShaderHandle vertex_shader =
            nvrhi_device->createShader(nvrhi::ShaderDesc(nvrhi::ShaderType::Vertex), g_unlit_main_vs_dxbc, sizeof(g_unlit_main_vs_dxbc));

        nvrhi::VertexAttributeDesc attributes[] = {
            nvrhi::VertexAttributeDesc()
                .setName("POSITION")
                .setFormat(nvrhi::Format::RGB32_FLOAT)
                .setOffset(offsetof(Vertex3D, position))
                .setElementStride(sizeof(Vertex3D)),
            nvrhi::VertexAttributeDesc()
                .setName("NORMAL")
                .setFormat(nvrhi::Format::RGB32_FLOAT)
                .setOffset(offsetof(Vertex3D, normal))
                .setElementStride(sizeof(Vertex3D)),
        };

        nvrhi::InputLayoutHandle input_layout = nvrhi_device->createInputLayout(attributes, uint32_t(std::size(attributes)), vertex_shader);

        nvrhi::ShaderHandle pixel_shader =
            nvrhi_device->createShader(nvrhi::ShaderDesc(nvrhi::ShaderType::Pixel), g_unlit_main_ps_dxbc, sizeof(g_unlit_main_ps_dxbc));

        auto binding_layout_desc =
            nvrhi::BindingLayoutDesc().setVisibility(nvrhi::ShaderType::All).addItem(nvrhi::BindingLayoutItem::VolatileConstantBuffer(0));

        nvrhi::BindingLayoutHandle binding_layout = nvrhi_device->createBindingLayout(binding_layout_desc);

        const auto pipeline_desc = nvrhi::GraphicsPipelineDesc()
                                       .setInputLayout(input_layout)
                                       .setVertexShader(vertex_shader)
                                       .setPixelShader(pixel_shader)
                                       .addBindingLayout(binding_layout);

        nvrhi::GraphicsPipelineHandle graphicsPipeline = nvrhi_device->createGraphicsPipeline(pipeline_desc, framebuffer);
    }

    self = this;
}

MaterialManager::~MaterialManager() {
    PRECONDITION(self != nullptr);
    self = nullptr;
}

} // namespace reach