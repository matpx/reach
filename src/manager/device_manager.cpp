#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/device_manager.hpp>
#include <manager/window_manager.hpp>
#include <nvrhi/d3d11.h>
#include <nvrhi/validation.h>
#include <utils/conditions.hpp>

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace reach {

class MessageCallback final : public nvrhi::IMessageCallback {
        virtual void message(nvrhi::MessageSeverity severity, const char *messageText) { LOG_ERROR(messageText); };
};

static DeviceManager *self = nullptr;

DeviceManager &DeviceManager::get() { return *self; }

DeviceManager::DeviceManager() {
    PRECONDITION(self == nullptr);

    const glm::ivec2 width_height = WindowManager::get().get_window_width_height();

    init_d3d11(width_height);
    init_nvrhi(width_height);

    self = this;
}

DeviceManager::~DeviceManager() {
    PRECONDITION(self != nullptr);

    back_buffer->Release();
    swap_chain->Release();
    d3d11_device->Release();
    d3d11_device_context->Release();

    self = nullptr;
}

void DeviceManager::init_d3d11(const glm::ivec2 &width_height) {
    HWND hwnd = glfwGetWin32Window(WindowManager::get().get_glfw_window());

    DXGI_MODE_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(DXGI_MODE_DESC));

    buffer_desc.Width = width_height.x;
    buffer_desc.Height = width_height.y;
    buffer_desc.RefreshRate.Numerator = 60;
    buffer_desc.RefreshRate.Denominator = 1;
    buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swap_chain_desc.BufferDesc = buffer_desc;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = hwnd;
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT device_flags = 0;

#ifndef NDEBUG
    LOG_INFO("enable d3d11 D3D11_CREATE_DEVICE_DEBUG");
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, device_flags, NULL, NULL, D3D11_SDK_VERSION,
                                               &swap_chain_desc, &swap_chain, &d3d11_device, NULL, &d3d11_device_context);
    POSTCONDITION(SUCCEEDED(hr));

    hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&back_buffer);
    POSTCONDITION(SUCCEEDED(hr));

    hr = d3d11_device->CreateRenderTargetView(back_buffer, NULL, &renderTargetView);
    POSTCONDITION(SUCCEEDED(hr));

    d3d11_device_context->OMSetRenderTargets(1, &renderTargetView, NULL);

    depth_stencil = nullptr;
    D3D11_TEXTURE2D_DESC depth_desc;
    depth_desc.Width = buffer_desc.Width;
    depth_desc.Height = buffer_desc.Height;
    depth_desc.MipLevels = 1;
    depth_desc.ArraySize = 1;
    depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_desc.SampleDesc.Count = 1;
    depth_desc.SampleDesc.Quality = 0;
    depth_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_desc.CPUAccessFlags = 0;
    depth_desc.MiscFlags = 0;

    hr = d3d11_device->CreateTexture2D(&depth_desc, NULL, &depth_stencil);
    POSTCONDITION(SUCCEEDED(hr));
}

void DeviceManager::init_nvrhi(const glm::ivec2 &width_height) {
    static MessageCallback message_callback;

    nvrhi::d3d11::DeviceDesc device_desc;
    device_desc.messageCallback = static_cast<nvrhi::IMessageCallback *>(&message_callback);
    device_desc.context = d3d11_device_context;

    nvrhi_device = nvrhi::d3d11::createDevice(device_desc);

#ifndef NDEBUG
    LOG_INFO("enable nvrhi validation");
    nvrhi::DeviceHandle nvrhi_validation_layer = nvrhi::validation::createValidationLayer(nvrhi_device);
    nvrhi_device = nvrhi_validation_layer;
#endif

    const auto swap_chain_texture_desc = nvrhi::TextureDesc()
                                             .setDimension(nvrhi::TextureDimension::Texture2D)
                                             .setFormat(nvrhi::Format::RGBA8_UNORM)
                                             .setWidth(width_height.x)
                                             .setHeight(width_height.y)
                                             .setIsRenderTarget(true)
                                             .setDebugName("Swap Chain Image");

    swap_chain_texture =
        nvrhi_device->createHandleForNativeTexture(nvrhi::ObjectTypes::D3D11_Resource, back_buffer, swap_chain_texture_desc);

    const auto depth_texture_desc = nvrhi::TextureDesc()
                                        .setWidth(static_cast<uint32_t>(width_height.x))
                                        .setHeight(static_cast<uint32_t>(width_height.y))
                                        .setFormat(nvrhi::Format::D24S8)
                                        .setDimension(nvrhi::TextureDimension::Texture2D)
                                        .setDebugName("Depth Buffer")
                                        .setIsRenderTarget(true)
                                        .setUseClearValue(true)
                                        .setInitialState(nvrhi::ResourceStates::DepthWrite)
                                        .setKeepInitialState(true);

    depth_texture = nvrhi_device->createHandleForNativeTexture(nvrhi::ObjectTypes::D3D11_Resource, depth_stencil, depth_texture_desc);

    const auto framebufferDesc = nvrhi::FramebufferDesc().addColorAttachment(swap_chain_texture).setDepthAttachment(depth_texture);
    framebuffer = nvrhi_device->createFramebuffer(framebufferDesc);
}

void DeviceManager::swap_d3d11() { swap_chain->Present(0, 0); }

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
