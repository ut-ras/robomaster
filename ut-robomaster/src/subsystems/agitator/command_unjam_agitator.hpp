#pragma once

#include "tap/control/command.hpp"

#include "subsystems/agitator/agitator_subsystem.hpp"

#include "robots/robot_constants.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::agitator::AgitatorSubsystem;

class CommandUnjamAgitator : public tap::control::Command
{
public:
    CommandUnjamAgitator(src::Drivers *drivers, AgitatorSubsystem *agitator)
        : drivers(drivers),
          agitator(agitator)
    {
        addSubsystemRequirement(agitator);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "unjam agitator command"; }

private:
    src::Drivers *drivers;
    AgitatorSubsystem *agitator;
};  // class CommandFireContinuous
}  // namespace commands