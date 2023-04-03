#ifndef COMMAND_FIRE_CONTINUOUS_HPP_
#define COMMAND_FIRE_CONTINUOUS_HPP_

#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "shooter_subsystem.hpp"

namespace subsystems
{
namespace shooter
{
class CommandFireContinuous : public tap::control::Command
{
public:
    CommandFireContinuous(ShooterSubsystem *sub, src::Drivers *drivers)
        : subsystem(sub),
          drivers(drivers)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "shooter on command"; }

private:
    ShooterSubsystem *subsystem;
    src::Drivers *drivers;

};  // class CommandFireContinuous
}  // namespace shooter
}  // namespace subsystems

#endif  // SHOOTER_ON_COMMAND_HPP_