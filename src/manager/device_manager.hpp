#pragma once

#include <manager/manager.hpp>


namespace reach {

class DeviceManager final : public Manager {
    public:
        DeviceManager();
        ~DeviceManager();
};

} // namespace reach