#include "lvgl_controller.h"
#include "logger_core.h"
#include <algorithm>

namespace core0 {

LvglController::LvglController(const Config &cfg) : cfg_(cfg) {}

void LvglController::Init(LcdBus &bus, PanelInterface &panel)
{
    LOG_SCOPE();

    bus_   = &bus;
    panel_ = &panel;

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
    // 書き込み先の矩形を設定
    panel_->SetAddrWindow(*bus_, area->x1, area->y1, area->x2, area->y2);

    // T.B.D. CS制御は不要かも（なくても同じ動作をする。一旦コメントアウトしておく。）
    // 連続データ送出（CSは保持）
    //bus_->BeginStream();

    const uint32_t w        = static_cast<uint32_t>(area->x2 - area->x1 + 1);
    const uint32_t h        = static_cast<uint32_t>(area->y2 - area->y1 + 1);
    const uint32_t total_px = w * h;

    // 送出チャンク（px単位）。scratch_ は 2B/px なので /2 が最大px
    const uint32_t max_chunk_px = static_cast<uint32_t>(sizeof(scratch_) / 2);
    const uint32_t chunk_px     = std::max<uint32_t>(16, std::min<uint32_t>(cfg_.flushChunkPixels, max_chunk_px));

    const uint8_t *src  = px;  // LVGLの16bppは通常 [LSB][MSB] 並び
    uint32_t       sent = 0;

    while (sent < total_px) {
        const uint32_t n = std::min<uint32_t>(chunk_px, total_px - sent);

        if (cfg_.swapRgb565Bytes) {
            // [LSB][MSB] → [MSB][LSB] に並べ替えて送る（SPIはMSBファースト）
            uint8_t       *q = scratch_;
            const uint8_t *s = src;
            for (uint32_t i = 0; i < n; ++i) {
                q[0] = s[1];  // MSB
                q[1] = s[0];  // LSB
                q += 2;
                s += 2;
            }
            bus_->Data(scratch_, n * 2);
            src += n * 2;
        } else {
            // すでに [MSB][LSB] 並びのときのみ素通し
            bus_->Data(src, n * 2);
            src += n * 2;
        }

        sent += n;
    }

    //bus_->WaitTxDone();
    //bus_->EndStream();

    lv_disp_flush_ready(d);
}
}  // namespace core0
