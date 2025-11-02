#include "lvgl_controller.h"
#include "logger_core.h"
#include <algorithm>

namespace core0 {

LvglController::LvglController(const Config &cfg) : cfg_(cfg) {}

void LvglController::Init(LcdBus &bus)
{
    LOG_SCOPE();

    bus_ = &bus;

    lv_init();

    // LVGL display 作成・登録（flush/buffers/user_data）
    disp_ = lv_display_create(cfg_.hor, cfg_.ver);
    lv_display_set_color_format(disp_, LV_COLOR_FORMAT_RGB565);
    lv_display_set_rotation(disp_, LV_DISPLAY_ROTATION_0);
    lv_display_set_flush_cb(disp_, &LvglController::FlushThunk);
    lv_display_set_buffers(disp_, drawbuf_, nullptr, static_cast<uint32_t>(cfg_.hor) * cfg_.ver * 2,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_user_data(disp_, this);
}

void LvglController::SetRotation(lv_display_rotation_t rot) { lv_display_set_rotation(disp_, rot); }

void LvglController::FlushThunk(lv_display_t *d, const lv_area_t *a, uint8_t *px)
{
    auto *self = static_cast<LvglController *>(lv_display_get_user_data(d));
    if (!self) {
        return;
    }
    LOGI("called flushThunk");
    self->FlushImpl(d, a, px);
}

void LvglController::FlushImpl(lv_display_t *d, const lv_area_t *area, uint8_t *px)
{
    // 矩形設定
    uint8_t col[4] = {uint8_t(area->x1 >> 8), uint8_t(area->x1), uint8_t(area->x2 >> 8), uint8_t(area->x2)};
    uint8_t row[4] = {uint8_t(area->y1 >> 8), uint8_t(area->y1), uint8_t(area->y2 >> 8), uint8_t(area->y2)};

    bus_->Cmd(0x2A);
    bus_->Data(col, 4);
    bus_->Cmd(0x2B);
    bus_->Data(row, 4);
    bus_->Cmd(0x2C);  // RAMWR

    const uint32_t w     = static_cast<uint32_t>(area->x2 - area->x1 + 1);
    const uint32_t h     = static_cast<uint32_t>(area->y2 - area->y1 + 1);
    const uint32_t total = w * h;

    const uint32_t chunk    = std::max<uint32_t>(16, cfg_.flushChunkPixels);
    const uint32_t maxChunk = sizeof(scratch_) / 2;

    auto    *p    = reinterpret_cast<uint16_t *>(px);
    uint32_t sent = 0;

    while (sent < total) {
        const uint32_t want = std::min<uint32_t>({chunk, maxChunk, total - sent});
        uint8_t       *q    = scratch_;

        if (cfg_.swapRgb565Bytes) {
            for (uint32_t i = 0; i < want; ++i) {
                uint16_t rgb = *p++;
                *q++         = static_cast<uint8_t>(rgb & 0xFF);  // LSB
                *q++         = static_cast<uint8_t>(rgb >> 8);    // MSB
            }
        } else {
            for (uint32_t i = 0; i < want; ++i) {
                uint16_t rgb = *p++;
                *q++         = static_cast<uint8_t>(rgb >> 8);    // MSB
                *q++         = static_cast<uint8_t>(rgb & 0xFF);  // LSB
            }
        }
        bus_->Data(scratch_, want * 2);
        sent += want;
    }

    lv_disp_flush_ready(d);
}
}  // namespace core0
