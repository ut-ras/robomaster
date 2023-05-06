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
    CommandFireContinuous(src::Drivers *drivers, ShooterSubsystem *shooter)
        : drivers(drivers),
          shooter(shooter)
    {
        addSubsystemRequirement(shooter);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "shooter on command"; }

private:
    src::Drivers *drivers;
    ShooterSubsystem *shooter;

};  // class CommandFireContinuous
}  // namespace shooter
}  // namespace subsystems

#endif