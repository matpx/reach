#pragma once

#include <concurrentqueue.h>
#include <data/mesh_data.hpp>
#include <manager/manager.hpp>
#include <mat4x4.hpp>
#include <span>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace reach {

struct MeshData;
struct MeshComponent;
struct MaterialComponent;
struct TransformComponent;

class DeviceManager final : public Manager {
    private:
        bool pass_is_active = false;

        IDXGISwapChain *SwapChain;
        ID3D11Device *d3d11Device;
        ID3D11DeviceContext *d3d11DeviveContext;
        ID3D11RenderTargetView *renderTargetView;

        void init_d3d11();
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
};

} // namespace reach