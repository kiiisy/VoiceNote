#pragma once
#include "xgpiops.h"
#include <cstdint>

namespace core0 {

class GpioPs
{
public:
    explicit GpioPs(uint32_t base_addr) : base_addr_(base_addr) {}

    static GpioPs &GetInstance()
    {
        static GpioPs inst;
        return inst;
    }

    int  Init();
    void Configure(const uint32_t base_addr) { base_addr_ = base_addr; };
    void WritePin(uint32_t mio, uint32_t level);
    void SetOutput(uint32_t mio);

    GpioPs(const GpioPs &)            = delete;
    GpioPs &operator=(const GpioPs &) = delete;

private:
    GpioPs() = default;
    uint32_t base_addr_;
    XGpioPs  gpio_{};
};
}  // namespace core0
