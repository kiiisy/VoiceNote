// st7789_panel.h
#pragma once
#include "panel_interface.h"
class LcdBus;

namespace core0 {

class St7789Panel final : public PanelInterface
{
public:
    void Init(const LcdBus &bus) noexcept override;
    void Reset(const LcdBus &bus) noexcept override;
    void SetRotation(const LcdBus &bus, uint8_t madctl) noexcept override;
    void SetAddrWindow(const LcdBus &b, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) noexcept override;

    bool SelfTestPattern(LcdBus &b, bool use_bgr = false) noexcept;

private:
    static constexpr uint8_t CMD_SWRESET  = 0x01;
    static constexpr uint8_t CMD_SLP_OUT  = 0x11;
    static constexpr uint8_t CMD_DISP_ON  = 0x29;
    static constexpr uint8_t CMD_MADCTL   = 0x36;
    static constexpr uint8_t CMD_COLMOD   = 0x3A;
    static constexpr uint8_t CMD_CASET    = 0x2A;  // column address set
    static constexpr uint8_t CMD_RASET    = 0x2B;  // row address set
    static constexpr uint8_t CMD_RAMWR    = 0x2C;
    static constexpr uint8_t CMD_INVON    = 0x21;  // 反転ON
    static constexpr uint8_t CMD_INVOFF   = 0x20;  // 反転OFF
    static constexpr uint8_t COLMOD_16BPP = 0x55;  // RGB565

    static void CMD(const LcdBus &b, uint8_t c) noexcept;
    static void DAT(const LcdBus &b, uint8_t d) noexcept;
    static void DAT(const LcdBus &b, const uint8_t *p, size_t n) noexcept;

    // 240x320 フル領域
    static void SetFullAddr(const LcdBus &b) noexcept;

    uint8_t                   rot_   = 0;    // MADCTL に渡した値を保持
    static constexpr uint16_t PHYS_W = 240;  // 物理 X 範囲
    static constexpr uint16_t PHYS_H = 320;  // 物理 Y 範囲
};

}  // namespace core0
