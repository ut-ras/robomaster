#include "flywheel_subsystem.hpp"

namespace subsystems::flywheel
{
#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
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
#elif defined(TARGET_HERO)
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

void FlywheelSubsystem::initialize() {
    for (int i = 0; i < FLYWHEELS; i++) {
        motors[i].initialize();
    }
}

void FlywheelSubsystem::refresh() {
    bool killSwitch = drivers->isKillSwitched();

    #if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        for (int i = 0; i < FLYWHEELS; i++) {
            motors[i].setActive(!killSwitch);
        }

        // LEFT FLYWHEELS
        motors[0].update(launchSpeedLeft);
        motors[2].update(launchSpeedLeft);

        // RIGHT FLYWHEELS
        motors[1].update(launchSpeedRight);
        motors[3].update(launchSpeedRight);

    #elif defined(TARGET_HERO)
        for (int i = 0; i < FLYWHEELS; i++) {
            motors[i].setActive(!killSwitch);
            motors[i].update(launchSpeedHero);
        }
    #endif
}

void FlywheelSubsystem::setLaunchSpeed(float speed) {
    #if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        launchSpeedLeft = speed;
        launchSpeedRight = speed;
    #elif defined(TARGET_HERO)
        launchSpeedHero = speed;
    #endif
}

void FlywheelSubsystem::setLaunchSpeed(float leftSpeed, float rightSpeed) {
    #if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        launchSpeedLeft = leftSpeed;
        launchSpeedRight = rightSpeed;
    #elif defined(TARGET_HERO)
        launchSpeedHero = leftSpeed;
    #endif
}
}
