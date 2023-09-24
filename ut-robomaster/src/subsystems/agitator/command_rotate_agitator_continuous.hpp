#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/agitator/agitator_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::agitator::AgitatorSubsystem;

class CommandRotateAgitatorContinuous : public tap::control::Command
{
public:
    CommandRotateAgitatorContinuous(src::Drivers *drivers, AgitatorSubsystem *agitator)
        : drivers(drivers),
          agitator(agitator)
    {
        addSubsystemRequirement(agitator);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "rotate agitator continuous command"; }

private:
    src::Drivers *drivers;
    AgitatorSubsystem *agitator;
};  // class CommandFireContinuous
}  // namespace commands