#pragma once
#include "panel_interface.h"
#include <cstdint>

namespace core0 {

class Ili9341Panel final : public PanelInterface
{
public:
    void Init(const LcdBus &bus) override;
    void Reset(const LcdBus &bus) override;
    void SetRotation(const LcdBus &bus, uint8_t madctl) override;
};
}  // namespace core0
