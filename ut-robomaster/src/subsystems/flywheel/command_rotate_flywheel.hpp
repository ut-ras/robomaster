#pragma once

#include "tap/control/command.hpp"

#include "subsystems/flywheel/flywheel_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::flywheel::FlywheelSubsystem;

class CommandRotateFlywheel : public tap::control::Command
{
public:
    CommandRotateFlywheel(src::Drivers *drivers, FlywheelSubsystem *flywheel)
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

};  // class CommandRotateFlywheel
}  // namespace commands