#pragma once

#include "drivers.hpp"

namespace power_limiter
{

class PowerLimiter
{
public:
    PowerLimiter(
        const tap::Drivers *drivers,
        float energyBufferLimitThreshold,
        float energyBufferCritThreshold);

    float getPowerLimitRatio();

private:
    const tap::Drivers *drivers;
    const float ENERGY_BUFFER_LIMIT_THRESHOLD;
    const float ENERGY_BUFFER_CRIT_THRESHOLD;
    float energyBuffer = 0;
};
}  // namespace power_limiter