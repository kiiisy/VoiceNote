#pragma once
#include "xspips.h"
#include <cstdint>

namespace core0 {

class SpiPs
{
public:
    struct Config
    {
        uint32_t base_addr_;
        uint8_t  clk_prescaler;
        uint8_t  slave_select;
        uint32_t options;
    };

    explicit SpiPs(const Config &c) : cfg_(c) {}

    int Init();
    int Transfer(const uint8_t *, uint8_t *, uint32_t);

private:
    Config cfg_{};
    XSpiPs spi_{};
};

}  // namespace core0
