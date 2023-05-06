#ifndef COMMAND_FIRE_ONCE_HPP_
#define COMMAND_FIRE_ONCE_HPP_

#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "shooter_subsystem.hpp"

namespace subsystems
{
namespace shooter
{
class CommandFireOnce : public tap::control::Command
{
public:
    CommandFireOnce(src::Drivers *drivers, ShooterSubsystem *shooter)
        : drivers(drivers),
          shooter(shooter)
    {
        addSubsystemRequirement(shooter);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "shooter single fire command"; }

private:
    src::Drivers *drivers;
    ShooterSubsystem *shooter;
};  // class CommandFireOnce
}  // namespace shooter
}  // namespace subsystems

#endif