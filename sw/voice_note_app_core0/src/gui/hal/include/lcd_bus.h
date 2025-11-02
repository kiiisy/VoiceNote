#pragma once
#include "gpio_ps.h"
#include "spi_ps.h"
#include <cstdint>

namespace core0 {

class SpiPs;
class GpioPs;

#if defined(__GNUC__)
#define LCD_ALWAYS_INLINE __attribute__((always_inline)) inline
#else
#define LCD_ALWAYS_INLINE inline
#endif

class LcdBus
{
public:
    enum class ResetPolarity : uint8_t
    {
        ActiveLow,
        ActiveHigh
    };

    LcdBus(SpiPs &spi, GpioPs &gpio, uint32_t dc, uint32_t rst, ResetPolarity pol = ResetPolarity::ActiveLow)
        : spi_(spi), gpio_(gpio), dc_(dc), rst_(rst), pol_(pol)
    {
    }

    LCD_ALWAYS_INLINE void Cmd(uint8_t c) const noexcept
    {
        gpio_.WritePin(dc_, 0);
        spi_.Transfer(&c, nullptr, 1);
    }

    LCD_ALWAYS_INLINE void Data(const uint8_t *d, uint32_t n) const noexcept
    {
        gpio_.WritePin(dc_, 1);
        spi_.Transfer(d, nullptr, n);
    }

    LCD_ALWAYS_INLINE void Data(uint8_t v) const noexcept { Data(&v, 1); }

    LCD_ALWAYS_INLINE void ResetAssert() const noexcept
    {
        const uint32_t level = (pol_ == ResetPolarity::ActiveLow) ? 0u : 1u;
        gpio_.WritePin(rst_, level);
    }
    LCD_ALWAYS_INLINE void ResetDeassert() const noexcept
    {
        const uint32_t level = (pol_ == ResetPolarity::ActiveLow) ? 1u : 0u;
        gpio_.WritePin(rst_, level);
    }

private:
    SpiPs        &spi_;
    GpioPs       &gpio_;
    uint32_t      dc_;   // DCピン番号
    uint32_t      rst_;  // RSTピン番号
    ResetPolarity pol_;
};
}  // namespace core0
