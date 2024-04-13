#pragma once

#include <concurrentqueue.h>
#include <data/mesh_data.hpp>
#include <manager/manager.hpp>
#include <mat4x4.hpp>
#include <nvrhi/nvrhi.h>
#include <span>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace reach {

struct MeshData;
struct MeshComponent;
struct MaterialComponent;
struct TransformComponent;

class DeviceManager final : public Manager {
    private:
        bool pass_is_active = false;

        IDXGISwapChain *swap_chain;
        ID3D11Device *d3d11_device;
        ID3D11DeviceContext *d3d11_device_context;
        ID3D11RenderTargetView *renderTargetView;
        ID3D11Texture2D *back_buffer;
        ID3D11Texture2D *depth_stencil;

        nvrhi::DeviceHandle nvrhi_device;
        nvrhi::TextureHandle swap_chain_texture;
        nvrhi::TextureHandle depth_texture;
        nvrhi::FramebufferHandle framebuffer;

        void init_d3d11(const glm::ivec2 &width_height);
        void init_nvrhi(const glm::ivec2 &width_height);

        void swap_d3d11();

    public:
        [[nodiscard]] static DeviceManager &get();

        DeviceManager();
        ~DeviceManager();

        void upload_meshdata(MeshData &mesh_data);

        void begin_main_pass();
        void draw_mesh(const glm::mat4 &model_view_projection, const MaterialComponent &material_component,
                       const MeshComponent &mesh_component);
        void draw_immediate(const glm::mat4 &projection, const std::span<const Vertex2D> vertex_data,
                            const MaterialComponent &material_component);
        void finish_main_pass();

        [[nodiscard]] nvrhi::DeviceHandle &get_nvrhi_device() { return nvrhi_device; }
        [[nodiscard]] nvrhi::FramebufferHandle &get_nvrhi_framebuffer() { return framebuffer; }
};

} // namespace reach