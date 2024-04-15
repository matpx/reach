#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <data/mesh_data.hpp>
#include <gtc/type_ptr.hpp>
#include <manager/device_manager.hpp>
#include <manager/window_manager.hpp>
#include <nvrhi/d3d11.h>
#include <nvrhi/utils.h>
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

    LOG_INFO("init device");

    const glm::ivec2 width_height = WindowManager::get().get_window_width_height();

    init_d3d11(width_height);
    init_nvrhi(width_height);

    self = this;
}

DeviceManager::~DeviceManager() {
    PRECONDITION(self != nullptr);

    depth_stencil->Release();
    back_buffer->Release();
    renderTargetView->Release();
    swap_chain->Release();
    d3d11_device_context->Release();
    d3d11_device->Release();

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

    UINT d3d11_device_flags = 0;

#ifndef NDEBUG
    LOG_INFO("set d3d11_device_flags = D3D11_CREATE_DEVICE_DEBUG");

    d3d11_device_flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, d3d11_device_flags, NULL, NULL, D3D11_SDK_VERSION,
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
    LOG_INFO("create nvrhi validation layer");

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

    const auto transform_constant_buffer_desc = nvrhi::BufferDesc()
                                                    .setByteSize(sizeof(glm::mat4::value_type) * 16)
                                                    .setIsConstantBuffer(true)
                                                    .setIsVolatile(true)
                                                    .setMaxVersions(16);
    transform_constant_buffer = nvrhi_device->createBuffer(transform_constant_buffer_desc);

    frame_command_list = nvrhi_device->createCommandList();
}

void DeviceManager::swap_d3d11() { swap_chain->Present(0, 0); }

void DeviceManager::upload_meshdata(MeshData &mesh_data) {
    PRECONDITION(pass_is_active == false);

    LOG_DEBUG("updating mesh_data: {}", mesh_data.debug_name);

    const size_t vertex_buffer_bytesize = mesh_data.vertex_data.size() * sizeof(decltype(mesh_data.vertex_data)::value_type);
    auto vertex_buffer_desc = nvrhi::BufferDesc()
                                  .setByteSize(vertex_buffer_bytesize)
                                  .setIsVertexBuffer(true)
                                  .setInitialState(nvrhi::ResourceStates::VertexBuffer)
                                  .setKeepInitialState(true)
                                  .setDebugName("Vertex Buffer");

    mesh_data.vertex_buffer = nvrhi_device->createBuffer(vertex_buffer_desc);

    const size_t index_buffer_bytesize = mesh_data.index_data.size() * sizeof(decltype(mesh_data.index_data)::value_type);
    auto index_buffer_desc = nvrhi::BufferDesc()
                                 .setByteSize(index_buffer_bytesize)
                                 .setIsIndexBuffer(true)
                                 .setInitialState(nvrhi::ResourceStates::IndexBuffer)
                                 .setKeepInitialState(true)
                                 .setDebugName("Index Buffer");

    mesh_data.index_buffer = nvrhi_device->createBuffer(index_buffer_desc);

    nvrhi::CommandListHandle upload_command_list = nvrhi_device->createCommandList();
    upload_command_list->open();
    upload_command_list->writeBuffer(mesh_data.vertex_buffer, mesh_data.vertex_data.data(), vertex_buffer_bytesize);
    upload_command_list->writeBuffer(mesh_data.index_buffer, mesh_data.index_data.data(), index_buffer_bytesize);
    upload_command_list->close();
    nvrhi_device->executeCommandList(upload_command_list);
}

void DeviceManager::begin_main_pass() {
    PRECONDITION(pass_is_active == false);

    pass_is_active = true;

    frame_command_list->open();

    nvrhi::utils::ClearColorAttachment(frame_command_list, framebuffer, 0, nvrhi::Color(1.0f));
    nvrhi::utils::ClearDepthStencilAttachment(frame_command_list, framebuffer, 1.0f, 0);
}

void DeviceManager::draw_mesh(const glm::mat4 &model_view_projection, const MaterialComponent &material_component,
                              const MeshComponent &mesh_component) {
    PRECONDITION(pass_is_active == true);

    std::span<const float> model_view_projection_ptr(glm::value_ptr(model_view_projection), 4 * 4);
    frame_command_list->writeBuffer(transform_constant_buffer, model_view_projection_ptr.data(), model_view_projection_ptr.size_bytes());

    const glm::ivec2 width_height = WindowManager::get().get_window_width_height();
    const auto graphics_state =
        nvrhi::GraphicsState()
            .setFramebuffer(framebuffer)
            .setViewport(nvrhi::ViewportState().addViewportAndScissorRect(
                nvrhi::Viewport(static_cast<float>(width_height.x), static_cast<float>(width_height.y))))
            .setPipeline(material_component.graphics_pipeline)
            .addBindingSet(material_component.binding_set)
            .addVertexBuffer(nvrhi::VertexBufferBinding().setBuffer(mesh_component.mesh_data->vertex_buffer))
            .setIndexBuffer(
                nvrhi::IndexBufferBinding().setBuffer(mesh_component.mesh_data->index_buffer).setFormat(nvrhi::Format::R32_UINT));

    frame_command_list->setGraphicsState(graphics_state);

    const auto draw_arguments =
        nvrhi::DrawArguments().setVertexCount(mesh_component.index_count).setStartIndexLocation(mesh_component.index_offset);
    frame_command_list->drawIndexed(draw_arguments);
}

void DeviceManager::draw_immediate(const glm::mat4 &projection, const std::span<const Vertex2D> vertex_data,
                                   const MaterialComponent &material_component) {

    const size_t required_bytesize = vertex_data.size_bytes();

    if (immediate_vertex_buffer == nullptr || immediate_vertex_buffer->getDesc().byteSize < required_bytesize) {
        LOG_DEBUG("recreate immediate_vertex_buffer");

        auto vertex_buffer_desc = nvrhi::BufferDesc()
                                      .setByteSize(required_bytesize)
                                      .setIsVertexBuffer(true)
                                      .setInitialState(nvrhi::ResourceStates::VertexBuffer)
                                      .setKeepInitialState(true)
                                      .setDebugName("Immediate Vertex Buffer");

        immediate_vertex_buffer = nvrhi_device->createBuffer(vertex_buffer_desc);
    }

    frame_command_list->writeBuffer(immediate_vertex_buffer, vertex_data.data(), required_bytesize);

    std::span<const float> model_view_projection_ptr(glm::value_ptr(projection), 4 * 4);
    frame_command_list->writeBuffer(transform_constant_buffer, model_view_projection_ptr.data(), model_view_projection_ptr.size_bytes());

    const glm::ivec2 width_height = WindowManager::get().get_window_width_height();
    const auto graphics_state = nvrhi::GraphicsState()
                                    .setFramebuffer(framebuffer)
                                    .setViewport(nvrhi::ViewportState().addViewportAndScissorRect(
                                        nvrhi::Viewport(static_cast<float>(width_height.x), static_cast<float>(width_height.y))))
                                    .setPipeline(material_component.graphics_pipeline)
                                    .addBindingSet(material_component.binding_set)
                                    .addVertexBuffer(nvrhi::VertexBufferBinding().setBuffer(immediate_vertex_buffer));

    frame_command_list->setGraphicsState(graphics_state);

    const auto draw_arguments = nvrhi::DrawArguments().setVertexCount(static_cast<uint32_t>(vertex_data.size()));
    frame_command_list->draw(draw_arguments);
}

void DeviceManager::finish_main_pass() {
    PRECONDITION(pass_is_active == true);

    frame_command_list->close();
    nvrhi_device->executeCommandList(frame_command_list);

    swap_d3d11();

    nvrhi_device->runGarbageCollection();

    pass_is_active = false;
}

} // namespace reach
