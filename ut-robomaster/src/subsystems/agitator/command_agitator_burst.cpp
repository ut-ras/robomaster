#include "command_agitator_burst.hpp"

namespace commands
{
void CommandAgitatorBurst::initialize() { initialPosition = agitator->getPosition(); }

void CommandAgitatorBurst::execute() { agitator->setBallsPerSecond(BALLS_PER_SEC); }

void CommandAgitatorBurst::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandAgitatorBurst::isFinished() const
{
    if (agitator->getPosition() - initialPosition >= numToFire / BALLS_PER_REV)
    {
        return true;
    }

    if (drivers->refSerial.getRefSerialReceivingData() &&
        power_limiter::getRemainingCooldown(drivers, barrelId) <= BARREL_HEAT_BUFFER)
    {
        return true;
    }

    return false;
}
}  // namespace commands