#pragma once

#include "drivers.hpp"

namespace power_limiter
{
enum BarrelId
{
    STANDARD1,  // 17ID1
    STANDARD2,  // 17ID2
    HERO        // 42
};

float getRemainingCooldown(src::Drivers *drivers, BarrelId id);
}  // namespace power_limiter