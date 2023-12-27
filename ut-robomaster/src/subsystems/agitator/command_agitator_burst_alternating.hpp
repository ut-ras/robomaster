#pragma once

#include "tap/control/command.hpp"
#include "tap/control/comprised_command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/agitator/agitator_subsystem.hpp"

#include "command_agitator_burst.hpp"
#include "drivers.hpp"

namespace commands
{
using subsystems::agitator::AgitatorSubsystem;

class CommandAgitatorBurstAlternating : public tap::control::ComprisedCommand
{
public:
    CommandAgitatorBurstAlternating(
        src::Drivers *drivers,
        AgitatorSubsystem *agitator1,
        AgitatorSubsystem *agitator2,
        CommandAgitatorBurst *burstCmd1,
        CommandAgitatorBurst *burstCmd2)
        : ComprisedCommand(drivers),
          burstCmd1(burstCmd1),
          burstCmd2(burstCmd2)
    {
        addSubsystemRequirement(agitator1);
        addSubsystemRequirement(agitator2);
        comprisedCommandScheduler.registerSubsystem(agitator1);
        comprisedCommandScheduler.registerSubsystem(agitator2);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "agitator alternating burst command"; }

private:
    CommandAgitatorBurst *burstCmd1;
    CommandAgitatorBurst *burstCmd2;
    bool isAlternate = false;
};

}  // namespace commands