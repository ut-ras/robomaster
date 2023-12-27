#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/agitator/agitator_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::agitator::AgitatorSubsystem;

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

};  // class CommandRotateAgitatorBurst

}  // namespace commands