#include "power_limiter.hpp"

namespace power_limiter {
    PowerLimiter::PowerLimiter(
    const tap::Drivers *drivers,
    float energyBufferLimitThreshold,
    float energyBufferCritThreshold)
    : drivers(drivers),
      ENERGY_BUFFER_LIMIT_THRESHOLD(energyBufferLimitThreshold),
      ENERGY_BUFFER_CRIT_THRESHOLD(energyBufferCritThreshold)
{
}

float PowerLimiter::getPowerLimitRatio()
{
    if (!drivers->refSerial.getRefSerialReceivingData()) {
        return 1.0f;
    }

    energyBuffer = drivers->refSerial.getRobotData().chassis.powerBuffer;
    
    if (energyBuffer < ENERGY_BUFFER_LIMIT_THRESHOLD) {
        float returnVal = (energyBuffer - ENERGY_BUFFER_CRIT_THRESHOLD) / ENERGY_BUFFER_LIMIT_THRESHOLD;
        if (returnVal < 0.0f) { return 0.0f; }
        else if (returnVal > 1.0f) { return 1.0f; }
        else { return returnVal; }
    }

    else {
        return 1.0f;
    }
}
}