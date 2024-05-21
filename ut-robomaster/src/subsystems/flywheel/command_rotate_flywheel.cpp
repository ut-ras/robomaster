#include "command_rotate_flywheel.hpp"

namespace commands
{

void CommandRotateFlywheel::initialize() {}

void CommandRotateFlywheel::execute()
{
    if (drivers->refSerial.getRefSerialReceivingData())
    {
        auto turretData = drivers->refSerial.getRobotData().turret;

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        uint16_t speedLimit = turretData.barrelSpeedLimit17ID1;
#elif defined(TARGET_HERO)
        uint16_t speedLimit = turretData.barrelSpeedLimit42;
#endif

        float launchSpeed = 0.0f;

        for (int i = 0; i < 3; i++)
        {
            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == speedLimit)
            {
                launchSpeed = FLYWHEEL_RPS_MAPPING[i].getSecond();
                break;
            }
        }

        flywheel->setLaunchSpeed(launchSpeed);
    }
    else
    {
        // NO REF SYSTEM DATA
        flywheel->setLaunchSpeed(DEFAULT_SPEED);
    }
}

void CommandRotateFlywheel::end(bool) { flywheel->setLaunchSpeed(0.0f); }

bool CommandRotateFlywheel::isFinished() const { return false; }
}  // namespace commands