// st7789_panel.cpp
#include "st7789_panel.h"
#include "algorithm"
#include "lcd_bus.h"
#include "sleep.h"

namespace core0 {

void St7789Panel::CMD(const LcdBus &b, uint8_t c) noexcept { b.Cmd(c); }
void St7789Panel::DAT(const LcdBus &b, uint8_t d) noexcept { b.Data(d); }
void St7789Panel::DAT(const LcdBus &b, const uint8_t *p, size_t n) noexcept { b.Data(p, n); }

void St7789Panel::Reset(const LcdBus &b) noexcept
{
    b.ResetDeassert();
    usleep(5 * 1000);
    b.ResetAssert();
    usleep(20 * 1000);
    b.ResetDeassert();
    usleep(5 * 1000);
}

void St7789Panel::SetFullAddr(const LcdBus &b) noexcept
{
    // CASET: X = 0..239
    CMD(b, CMD_CASET);
    uint8_t col[] = {0x00, 0x00, 0x00, 0xEF};
    DAT(b, col, sizeof(col));
    // RASET: Y = 0..319
    CMD(b, CMD_RASET);
    uint8_t row[] = {0x00, 0x00, 0x01, 0x3F};
    DAT(b, row, sizeof(row));
}

void St7789Panel::Init(const LcdBus &b) noexcept
{
    Reset(b);
    CMD(b, CMD_SWRESET);
    usleep(120 * 1000);

    // 16bpp
    CMD(b, CMD_COLMOD);
    DAT(b, COLMOD_16BPP);

    // ディスプレイ反転を有効にする
    CMD(b, CMD_INVON);

    // MADCTL は後で SetRotation で上書き（初期はBGRのみ）
    CMD(b, CMD_MADCTL);
    DAT(b, 0x00);

    SetFullAddr(b);

    CMD(b, CMD_SLP_OUT);
    usleep(120 * 1000);
    CMD(b, CMD_DISP_ON);
    usleep(20 * 1000);
}

void St7789Panel::SetRotation(const LcdBus &b, uint8_t madctl) noexcept
{
    CMD(b, CMD_MADCTL);
    DAT(b, madctl);
    rot_ = madctl;
    SetFullAddr(b);
}

void St7789Panel::SetAddrWindow(const LcdBus &b, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) noexcept
{
    uint8_t col[] = {uint8_t(x0 >> 8), uint8_t(x0), uint8_t(x1 >> 8), uint8_t(x1)};
    uint8_t row[] = {uint8_t(y0 >> 8), uint8_t(y0), uint8_t(y1 >> 8), uint8_t(y1)};

    CMD(b, CMD_CASET);
    DAT(b, col, 4);
    CMD(b, CMD_RASET);
    DAT(b, row, 4);
    CMD(b, CMD_RAMWR);
}

bool St7789Panel::SelfTestPattern(LcdBus &b, bool use_bgr) noexcept
{
    // MADCTL: 回転なし + RGB/BGR 選択のみ
    {
        uint8_t mad = 0x00;
        if (use_bgr)
            mad |= MADCTL_BGR;
        CMD(b, CMD_MADCTL);
        DAT(b, mad);
    }

    // 画面全域
    SetFullAddr(b);

    CMD(b, CMD_RAMWR);

    // ベタ塗り（赤→緑→青）
    auto fill = [&](uint16_t rgb565) {
        constexpr int      kTotalPx = PHYS_W * PHYS_H;  // 240 * 320
        constexpr int      kChunkPx = 512;
        alignas(4) uint8_t buf[kChunkPx * 2];

        // ST7789 は SPI MSB-first → MSB→LSB で詰める（swap しない）
        for (int i = 0; i < kChunkPx; ++i) {
            buf[2 * i + 0] = static_cast<uint8_t>(rgb565 >> 8);    // MSB
            buf[2 * i + 1] = static_cast<uint8_t>(rgb565 & 0xFF);  // LSB
        }

        int left = kTotalPx;
        while (left > 0) {
            int n = (left > kChunkPx) ? kChunkPx : left;
            DAT(b, buf, static_cast<size_t>(n * 2));
            left -= n;
        }
        usleep(100 * 1000);
    };

    fill(0xF800);  // Red
    usleep(10000);
    fill(0x07E0);  // Green
    usleep(10000);
    fill(0x001F);  // Blue
    return true;
}

}  // namespace core0
