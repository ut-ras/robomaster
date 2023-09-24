#pragma once

#include "tap/control/command.hpp"

#include "subsystems/flywheel/flywheel_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::flywheel::FlywheelSubsystem;

class CommandFlywheelOff : public tap::control::Command
{
public:
    CommandFlywheelOff(src::Drivers *drivers, FlywheelSubsystem *flywheel)
        : drivers(drivers),
          flywheel(flywheel)
    {
        addSubsystemRequirement(flywheel);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "rotate flywheel command"; }

private:
    src::Drivers *drivers;
    FlywheelSubsystem *flywheel;

};  // class CommandFlywheelOff
}  // namespace commands