#pragma once

#include <concurrentqueue.h>
#include <data/mesh_data.hpp>
#include <manager/manager.hpp>
#include <mat4x4.hpp>
#include <sokol_gfx.h>
#include <span>

namespace reach {

struct MeshData;
struct MeshComponent;
struct MaterialComponent;
struct TransformComponent;

class DeviceManager final : public Manager {
    private:
        bool pass_is_active = false;

        sg_buffer_desc immediate_buffer_desc = {};
        sg_buffer immediate_buffer = {};

        moodycamel::ConcurrentQueue<sg_buffer> buffer_delete_queue;

    public:
        [[nodiscard]] static DeviceManager &get();

        DeviceManager();
        ~DeviceManager();

        void upload_meshdata(MeshData &mesh_data);
        void unload_meshdata(MeshData &mesh_data);
        void unload_meshdata_deferred(MeshData &mesh_data);

        void collect_gargabe();

        void begin_main_pass();
        void draw_mesh(const glm::mat4 &model_view_projection, const MaterialComponent &material_component,
                       const MeshComponent &mesh_component);
        void draw_immediate(const glm::mat4 &projection, const std::span<const Vertex2D> vertex_data,
                            const MaterialComponent &material_component);
        void finish_main_pass();
};

} // namespace reach