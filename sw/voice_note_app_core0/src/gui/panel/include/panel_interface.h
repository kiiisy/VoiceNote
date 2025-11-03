#pragma once
#include "gpio_ps.h"
#include "lcd_bus.h"
#include <cstdint>

namespace core0 {

class PanelInterface
{
public:
    virtual ~PanelInterface()                                   = default;
    virtual void Init(const LcdBus &bus)                        = 0;
    virtual void Reset(const LcdBus &bus)                       = 0;
    virtual void SetRotation(const LcdBus &bus, uint8_t madctl) = 0;
    virtual void SetAddrWindow(const LcdBus &bus, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) noexcept = 0;

    enum : uint8_t
    {
        MADCTL_MY  = 0x80,  // Row address order
        MADCTL_MX  = 0x40,  // Column address order
        MADCTL_MV  = 0x20,  // Row/Column exchange
        MADCTL_ML  = 0x10,
        MADCTL_BGR = 0x08,
        MADCTL_MH  = 0x04,
    };
};
}  // namespace core0
