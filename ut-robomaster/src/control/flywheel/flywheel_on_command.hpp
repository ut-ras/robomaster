#ifndef FLYWHEEL_ON_COMMAND_HPP_
#define FLYWHEEL_ON_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "flywheel_subsystem.hpp"

namespace control
{
namespace flywheel
{
class FlywheelOnCommand : public tap::control::Command
{
public:
    FlywheelOnCommand(FlywheelSubsystem *sub)
        : flywheel(sub)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "flywheel on command"; }

private:
    FlywheelSubsystem *flywheel;

};  // class FlywheelOnCommand
}  // namespace flywheel
}  // namespace control

#endif  // FLYWHEEL_ON_COMMAND_HPP_
