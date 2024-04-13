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

        nvrhi::ShaderHandle vertexShader = nvrhi_device->createShader(nvrhi::ShaderDesc(nvrhi::ShaderType::Vertex),
                                                                      g_immediate_main_vs_dxbc, sizeof(g_immediate_main_vs_dxbc));

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

        nvrhi::InputLayoutHandle inputLayout = nvrhi_device->createInputLayout(attributes, uint32_t(std::size(attributes)), vertexShader);

        nvrhi::ShaderHandle pixelShader = nvrhi_device->createShader(nvrhi::ShaderDesc(nvrhi::ShaderType::Pixel), g_immediate_main_ps_dxbc,
                                                                     sizeof(g_immediate_main_ps_dxbc));
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

    DeviceManager::get().draw_immediate(world.get<CameraComponent>(world.current_camera).ui_proj, immediate_data, immediate_material);

    immediate_data.resize(0);
}

} // namespace reach
