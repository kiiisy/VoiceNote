#include "gpio_ps.h"
#include "ili9341_panel.h"
#include "lcd_bus.h"
#include "lvgl_controller.h"
#include "sleep.h"
#include "spi_ps.h"

int main()
{
    core0::SpiPs spi(
        {XPAR_XSPIPS_0_BASEADDR, XSPIPS_CLK_PRESCALE_4, 0, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION});
    spi.Init();

    auto &gpio = core0::GpioPs::GetInstance();
    gpio.Configure(XPAR_XGPIOPS_0_BASEADDR);
    gpio.Init();
    const uint32_t DC_PIN = 0, RESET_PIN = 9;
    gpio.SetOutput(DC_PIN);
    gpio.SetOutput(RESET_PIN);

    core0::LcdBus lcd_bus(spi, gpio, DC_PIN, RESET_PIN, core0::LcdBus::ResetPolarity::ActiveLow);

    core0::Ili9341Panel ili9341;
    ili9341.Reset(lcd_bus);
    ili9341.Init(lcd_bus);
    ili9341.SetRotation(lcd_bus, core0::PanelInterface::MADCTL_MY | core0::PanelInterface::MADCTL_MV);

    core0::LvglController disp({.hor = 320, .ver = 240, .swapRgb565Bytes = true, .flushChunkPixels = 512});
    disp.Init(lcd_bus);

    auto *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello, Voice Note!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (1) {
        lv_timer_handler();
        usleep(5 * 1000);
    }
}
