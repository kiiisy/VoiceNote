#pragma once
#include "gpio_ps.h"
#include "lcd_bus.h"
#include <cstdint>
extern "C" {
#include "lvgl.h"
}

namespace core0 {

class LvglController
{
public:
    struct Config
    {
        uint16_t hor              = 240;
        uint16_t ver              = 320;
        bool     swapRgb565Bytes  = true;
        uint32_t flushChunkPixels = 512;  // 1回の送信ピクセル
    };

    explicit LvglController(const Config &cfg);
    void Init(LcdBus &bus);  // LVGLへの登録だけ担当
    void SetRotation(lv_display_rotation_t rot);

    lv_display_t *handle() const { return disp_; }

private:
    static void FlushThunk(lv_display_t *d, const lv_area_t *a, uint8_t *px);
    void        FlushImpl(lv_display_t *d, const lv_area_t *a, uint8_t *px);

    // 状態
    Config        cfg_{};
    LcdBus       *bus_{nullptr};
    lv_display_t *disp_{nullptr};

    // バッファ（.bss）
    static constexpr size_t kBufBytesMax = 240u * 320u * 2u;
    alignas(4) uint8_t drawbuf_[kBufBytesMax];

    // スクラッチをメンバー化（スタック節約）
    static constexpr uint32_t kScratchBytes = 1024 * 2;
    alignas(4) uint8_t scratch_[kScratchBytes];
};
}  // namespace core0
