#include "gpio_ps.h"
#include "logger_core.h"
#include "xil_printf.h"

namespace core0 {

int GpioPs::Init()
{
    LOG_SCOPE();

    XGpioPs_Config *cfg = XGpioPs_LookupConfig(base_addr_);
    if (!cfg) {
        xil_printf("GPIO cfg not found\r\n");
        return XST_FAILURE;
    }
    return XGpioPs_CfgInitialize(&gpio_, cfg, cfg->BaseAddr);
}

void GpioPs::WritePin(uint32_t mio, uint32_t level) { XGpioPs_WritePin(&gpio_, mio, level ? 1 : 0); }

void GpioPs::SetOutput(uint32_t mio)
{
    XGpioPs_SetDirectionPin(&gpio_, mio, 1);
    XGpioPs_SetOutputEnablePin(&gpio_, mio, 1);
}

}  // namespace core0
