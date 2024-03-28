#include "command_rotate_flywheel.hpp"

#include "tap/communication/sensors/buzzer/buzzer.hpp"

namespace commands
{

void CommandRotateFlywheel::initialize() {
	tap::buzzer::playNote(&drivers->pwm, 440);
}

void CommandRotateFlywheel::execute()
{
    if (drivers->refSerial.getRefSerialReceivingData())
    {
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        uint16_t refSystemLaunchSpeedLeft =
            drivers->refSerial.getRobotData().turret.barrelSpeedLimit17ID1;
        uint16_t refSystemLaunchSpeedRight =
            drivers->refSerial.getRobotData().turret.barrelSpeedLimit17ID2;

        float launchSpeedLeft = 0.0f;
        float launchSpeedRight = 0.0f;

        for (int i = 0; i < 3; i++)
        {
            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == refSystemLaunchSpeedLeft)
            {
                launchSpeedLeft = FLYWHEEL_RPS_MAPPING[i].getSecond();
            }

            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == refSystemLaunchSpeedRight)
            {
                launchSpeedRight = FLYWHEEL_RPS_MAPPING[i].getSecond();
            }

            if (launchSpeedLeft != 0.0f && launchSpeedRight != 0.0f)
            {
                break;
            }
        }

        flywheel->setLaunchSpeed(launchSpeedLeft, launchSpeedRight);

#elif defined(TARGET_HERO)
        uint16_t refSystemLaunchSpeed = drivers->refSerial.getRobotData().turret.barrelSpeedLimit42;
        float launchSpeedHero = 0.0f;

        for (int i = 0; i < 3; i++)
        {
            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == refSystemLaunchSpeed)
            {
                launchSpeedHero = FLYWHEEL_RPS_MAPPING[i].getSecond();
                break;
            }
        }

        flywheel->setLaunchSpeed(launchSpeedHero);
#endif
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
