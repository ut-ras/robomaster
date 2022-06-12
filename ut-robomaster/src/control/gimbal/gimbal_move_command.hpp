#ifndef GIMBAL_MOVE_COMMAND_HPP_
#define GIMBAL_MOVE_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "gimbal_subsystem.hpp"

class Drivers;

namespace control
{
namespace gimbal
{
class GimbalMoveCommand : public tap::control::Command
{
public:
    GimbalMoveCommand(tap::Drivers *drivers, GimbalSubsystem *sub);

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "gimbal move command"; }

private:
    tap::Drivers *drivers;
    GimbalSubsystem *gimbal;
};  // class GimbalDriveCommand
}  // namespace gimbal
}  // namespace control

#endif  // GIMBAL_DRIVE_COMMAND_HPP_
