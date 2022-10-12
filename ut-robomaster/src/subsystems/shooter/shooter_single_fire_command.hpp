#ifndef SHOOTER_SINGLE_FIRE_COMMAND_HPP_
#define SHOOTER_SINGLE_FIRE_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "shooter_subsystem.hpp"

namespace subsystems
{
namespace shooter
{
class ShooterSingleFireCommand : public tap::control::Command
{
public:
    ShooterSingleFireCommand(ShooterSubsystem *sub)
        : shooter(sub)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "shooter single fire command"; }

private:
    ShooterSubsystem *shooter;

};  // class ShooterOnCommand
}  // namespace shooter
}  // namespace control

#endif  // SHOOTER_SINGLE_FIRE_COMMAND_HPP_