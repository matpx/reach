#pragma once

namespace reach {

class NoCopy {
    public:
        NoCopy() = default;
        NoCopy(const NoCopy &) = delete;
};

} // namespace reach
