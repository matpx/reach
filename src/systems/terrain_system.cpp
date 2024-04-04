#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <gsl/util>
#include <rjm_mc.h>
#include <span>
#include <systems/terrain_system.hpp>
#include <world.hpp>

namespace reach::terrain_system {

constexpr float planet_max_radius = 10;

static float iso(const float *pos, float * /*extra*/, void * /*userparam*/) {
    float x = pos[0], y = pos[1], z = pos[2];
    return (x * x + y * y + z * z) - planet_max_radius - glm::epsilon<float>();
}

void update() {
    static bool first_run = true;

    if (first_run) {
        first_run = false;

        std::shared_ptr<MeshData> mesh_data = std::make_shared<MeshData>();

        const float bmin[3] = {-planet_max_radius / 2, -planet_max_radius / 2, -planet_max_radius / 2};
        const float bmax[3] = {+planet_max_radius / 2, +planet_max_radius / 2, +planet_max_radius / 2};
        const float resolution = planet_max_radius / 20.0f;

        McMesh mc_mesh = mcGenerate(bmin, bmax, resolution, iso, nullptr);

        const auto _ = gsl::finally([&mc_mesh]() { mcFree(&mc_mesh); });

        for (const McVertex &vert : std::span<McVertex>(mc_mesh.verts, mc_mesh.nverts)) {
            mesh_data->vertex_data.push_back(Vertex3D{
                .position = {vert.x, vert.y, vert.z},
                .normal = {vert.nx, vert.ny, vert.nz},
            });
        }

        for (const int &index : std::span<int>(mc_mesh.indices, mc_mesh.ntris * 3)) {
            mesh_data->index_data.push_back(index);
        }

        MeshComponent mesh_comp = {
            .mesh_data = mesh_data,
            .index_count = static_cast<uint32_t>(mesh_data->index_data.size()),
            .index_offset = 0,
        };

        const entt::entity chunk = World::current().create();
        World::current().emplace<TransformComponent>(chunk, TransformComponent{});
        World::current().emplace<MaterialComponent>(chunk, MaterialComponent{});
        World::current().emplace<MeshComponent>(chunk, mesh_comp);
    }
}

} // namespace reach::terrain_system
