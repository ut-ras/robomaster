#include "command_agitator_continuous.hpp"

namespace commands
{
void CommandAgitatorContinuous::initialize() {}

void CommandAgitatorContinuous::execute()
{
    float bps = BALLS_PER_SEC;

    if (drivers->refSerial.getRefSerialReceivingData() &&
        power_limiter::getRemainingCooldown(drivers, barrelId) <= BARREL_HEAT_BUFFER)
    {
        bps = 0.0f;
    }

    agitator->setBallsPerSecond(bps);
}

void CommandAgitatorContinuous::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandAgitatorContinuous::isFinished() const { return false; }
}  // namespace commands