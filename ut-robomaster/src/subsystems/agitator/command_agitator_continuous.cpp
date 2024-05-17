#include "command_agitator_continuous.hpp"

namespace commands
{

void CommandAgitatorContinuous::initialize()
{
    jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
    isJammed = false;
}

void CommandAgitatorContinuous::execute()
{
    if (isJammed)
    {
        if (unjammingTimeout.isExpired())
        {
            jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
            isJammed = false;
        }

        agitator->setBallsPerSecond(-UNJAM_SPEED);
    }
    else if (  // barrel overheat
        drivers->refSerial.getRefSerialReceivingData() &&
        power_limiter::getRemainingCooldown(drivers, barrelId) <= BARREL_HEAT_BUFFER)
    {
        agitator->setBallsPerSecond(0.0f);
    }
    else  // heat remaining and no jams
    {
        float speedRatio = abs(agitator->getVelocity()) / (BALLS_PER_SEC / BALLS_PER_REV);

        if (speedRatio > JAM_TRIGGER_RATIO)  // no jam
        {
            jamTriggerTimeout.restart(JAM_TRIGGER_DURATION * 1000);
        }
        else if (jamTriggerTimeout.isExpired())  // jam detected
        {
            unjammingTimeout.restart(UNJAM_DURATION * 1000);
            isJammed = true;
        }

        agitator->setBallsPerSecond(BALLS_PER_SEC);
    }
}

void CommandAgitatorContinuous::end(bool) { agitator->setBallsPerSecond(0.0f); }

bool CommandAgitatorContinuous::isFinished() const { return false; }
}  // namespace commands