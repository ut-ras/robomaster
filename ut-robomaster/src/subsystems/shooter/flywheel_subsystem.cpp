#include "flywheel_subsystem.hpp"

namespace subsystems::shooter
{
#if defined TARGET_STANDARD || TARGET_SENTRY
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_TL,
           CAN_SHOOTER,
           true,
           "flywheel top left",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_TR,
           CAN_SHOOTER,
           false,
           "flywheel top right",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_BL,
           CAN_SHOOTER,
           false,
           "flywheel bottom left",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_BR,
           CAN_SHOOTER,
           true,
           "flywheel bottom right",
           PID_FLYWHEEL}}
{
}
#elif defined TARGET_HERO
FlywheelSubsystem::FlywheelSubsystem(src::Drivers* drivers)
    : Subsystem(drivers),
      drivers(drivers),
      motors{
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_L,
           CAN_SHOOTER,
           true,
           "flywheel left",
           PID_FLYWHEEL},
          {drivers,
           M3508_NOGEARBOX,
           ID_FLYWHEEL_R,
           CAN_SHOOTER,
           false,
           "flywheel right",
           PID_FLYWHEEL}}
{
}
#endif

void FlywheelSubsystem::initialize()
{
    for (int i = 0; i < FLYWHEELS; i++)
    {
        motors[i].initialize();
    }
}

void FlywheelSubsystem::refresh()
{
    bool killSwitch = drivers->isKillSwitched();
    float launchSpeed = DEFAULT_SPEED;

    if (drivers->refSerial.getRefSerialReceivingData()) {

        #if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        float refSystemLaunchSpeedLeft = drivers->refSerial.getRobotData().turret.barrelSpeedLimit17ID1;
        float refSystemLaunchSpeedRight = drivers->refSerial.getRobotData().turret.barrelSpeedLimit17ID2;

        float launchSpeedLeft = 0.0f;
        float launchSpeedRight = 0.0f;

        for (int i = 0; i < 3; i++) {
            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == refSystemLaunchSpeedLeft) {
                launchSpeedLeft = FLYWHEEL_RPS_MAPPING[i].getSecond();
            }

            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == refSystemLaunchSpeedRight) {
                launchSpeedRight = FLYWHEEL_RPS_MAPPING[i].getSecond();
            }

            if (launchSpeedLeft != 0.0f && launchSpeedRight != 0.0f) {
                break;
            }
        }

        for (int i = 0; i < FLYWHEELS; i++) {
            motors[i].setActive(!killSwitch);
        }

        // LEFT FLYWHEELS
        motors[0].update(isActive ? launchSpeedLeft : 0.0f);
        motors[2].update(isActive ? launchSpeedLeft : 0.0f);

        // RIGHT FLYWHEELS
        motors[1].update(isActive ? launchSpeedRight : 0.0f);
        motors[3].update(isActive ? launchSpeedRight : 0.0f);


        #elif defined(TARGET_HERO)
        float refSystemLaunchSpeed = drivers->refSerial.getRobotData().turret.barrelSpeedLimit42;
        launchSpeedLeft = 0.0f;

        for (int i = 0; i < 3; i++) {
            if (FLYWHEEL_RPS_MAPPING[i].getFirst() == refSystemLaunchSpeed) {
                launchSpeed = FLYWHEEL_RPS_MAPPING[i].getSecond();
                break;
            }
        }

        for (int i = 0; i < FLYWHEELS; i++) {
            motors[i].setActive(!killSwitch);
            motors[i].update(isActive ? launchSpeed : 0.0f);
        }
        #endif
    }

    // NO REF SYSTEM DATA
    else {
        for (int i = 0; i < FLYWHEELS; i++) {
            motors[i].setActive(!killSwitch);
            motors[i].update(isActive ? launchSpeed : 0.0f);
        }
    }
}

void FlywheelSubsystem::setActive(bool active) { isActive = active; }
}  // namespace subsystems::shooter
