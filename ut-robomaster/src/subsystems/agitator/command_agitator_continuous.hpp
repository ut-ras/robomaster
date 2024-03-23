#pragma once

#include "tap/architecture/timeout.hpp"
#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/agitator/agitator_subsystem.hpp"
#include "utils/power_limiter/barrel_cooldown.hpp"

#include "drivers.hpp"

using tap::arch::MilliTimeout;

namespace commands
{
using power_limiter::BarrelId;
using subsystems::agitator::AgitatorSubsystem;

class CommandAgitatorContinuous : public tap::control::Command
{
public:
    CommandAgitatorContinuous(src::Drivers *drivers, AgitatorSubsystem *agitator, BarrelId barrelId)
        : drivers(drivers),
          agitator(agitator),
          barrelId(barrelId),
          timeout()
    {
        addSubsystemRequirement(agitator);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "agitator continuous command"; }

private:
    src::Drivers *drivers;
    AgitatorSubsystem *agitator;
    BarrelId barrelId;
    MilliTimeout timeout;
    MilliTimeout timeout2;
};
}  // namespace commands