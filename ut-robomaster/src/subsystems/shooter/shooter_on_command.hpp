#ifndef SHOOTER_ON_COMMAND_HPP_
#define SHOOTER_ON_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "shooter_subsystem.hpp"

namespace subsystems
{
namespace shooter
{
class ShooterOnCommand : public tap::control::Command
{
public:
    ShooterOnCommand(ShooterSubsystem *sub, src::Drivers *drivers)
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

};  // class ShooterOnCommand
}  // namespace shooter
}  // namespace subsystems

#endif  // SHOOTER_ON_COMMAND_HPP_