#include "spi_ps.h"
#include "logger_core.h"
#include "xil_printf.h"

namespace core0 {

int SpiPs::Init()
{
    LOG_SCOPE();

    auto *cfg = XSpiPs_LookupConfig(cfg_.base_addr_);
    if (!cfg) {
        xil_printf("SPI cfg not found\r\n");
        return XST_FAILURE;
    }

    int st = XSpiPs_CfgInitialize(&spi_, cfg, cfg->BaseAddress);
    if (st != XST_SUCCESS) {
        return st;
    }

    st = XSpiPs_SelfTest(&spi_);
    if (st != XST_SUCCESS) {
        return st;
    }

    st = XSpiPs_SetOptions(&spi_, cfg_.options);
    if (st != XST_SUCCESS) {
        return st;
    }

    st = XSpiPs_SetSlaveSelect(&spi_, cfg_.slave_select);
    if (st != XST_SUCCESS) {
        return st;
    }

    st = XSpiPs_SetClkPrescaler(&spi_, cfg_.clk_prescaler);
    if (st != XST_SUCCESS) {
        return st;
    }

    return XST_SUCCESS;
}

int SpiPs::Transfer(const uint8_t *tx, uint8_t *rx, uint32_t len)
{
    return XSpiPs_PolledTransfer(&spi_, const_cast<uint8_t *>(tx), rx, len);
}

}  // namespace core0
