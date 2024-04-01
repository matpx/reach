#include <data/mesh_data.hpp>
#include <manager/device_manager.hpp>

namespace reach {

MeshData::~MeshData() { DeviceManager::get().unload_meshdata_deferred(*this); }

} // namespace reach