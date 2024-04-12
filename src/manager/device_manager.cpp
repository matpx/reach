#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/device_manager.hpp>
#include <manager/window_manager.hpp>
#include <nvrhi/nvrhi.h>
#include <utils/conditions.hpp>

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace reach {

static DeviceManager *self = nullptr;

DeviceManager &DeviceManager::get() { return *self; }

DeviceManager::DeviceManager() {
    PRECONDITION(self == nullptr);

    init_d3d11();

    self = this;
}

DeviceManager::~DeviceManager() {
    PRECONDITION(self != nullptr);

    SwapChain->Release();
    d3d11Device->Release();
    d3d11DevCon->Release();

    self = nullptr;
}

void DeviceManager::init_d3d11() {
    const glm::ivec2 width_height = WindowManager::get().get_window_width_height();
    HWND hwnd = glfwGetWin32Window(WindowManager::get().get_glfw_window());

    // Describe our Buffer
    DXGI_MODE_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = width_height.x;
    bufferDesc.Height = width_height.y;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Create our SwapChain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc,
                                               &SwapChain, &d3d11Device, NULL, &d3d11DevCon);
    POSTCONDITION(SUCCEEDED(hr));

    // Create our BackBuffer
    ID3D11Texture2D *BackBuffer;
    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&BackBuffer);
    POSTCONDITION(SUCCEEDED(hr));

    // Create our Render Target
    hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
    BackBuffer->Release();
    POSTCONDITION(SUCCEEDED(hr));

    // Set our Render Target
    d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);
}

void DeviceManager::swap_d3d11() { SwapChain->Present(0, 0); }

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

    swap_d3d11();

    // sg_end_pass();
    // sg_commit();

    // collect_gargabe();
}

} // namespace reach
