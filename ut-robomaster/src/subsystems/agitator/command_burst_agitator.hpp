#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/agitator/agitator_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::agitator::AgitatorSubsystem;

// struct TurretCooldownPercentage
// {
//     int currentHeat1;
//     int currentHeat2;
//     int heatLimit1;
//     int heatLimit2;
//     int percentage1;
//     int percentage2;

//     float compare()
//     {
//         percentage1 = currentHeat1 * 1.0f / heatLimit1;
//         percentage2 = currentHeat2 * 1.0f / heatLimit2;
//         return percentage1 - percentage2;  // if positive, turret one has a higher cooldown
//                                            // if zero, both turrets have equivalent cooldown
//                                            // if negative, percentage 2 has a higher cooldown
//     }
// };

class CommandRotateAgitatorBurst : public tap::control::Command
{
public:
    CommandRotateAgitatorBurst(src::Drivers *drivers, AgitatorSubsystem *agitator, int ballCount)
        : drivers(drivers),
          agitator(agitator)
    {
        numToFire = ballCount;
        addSubsystemRequirement(agitator);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "rotate agitator burst command"; }

private:
    src::Drivers *drivers;
    AgitatorSubsystem *agitator;
    int numToFire;
    float beginPosition;
    float initialPosition;
    float targetPosition;
    bool isLeftTurret = false;
    // #if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    //     // TurretCooldownPercentage cooldownMeter;
    // #endif
};  // class CommandFireContinuous

}  // namespace commands