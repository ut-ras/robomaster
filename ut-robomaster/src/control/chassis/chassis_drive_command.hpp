#ifndef CHASSIS_DRIVE_COMMAND_HPP_
#define CHASSIS_DRIVE_COMMAND_HPP_

#include "tap/control/command.hpp"

#include "chassis_subsystem.hpp"

class Drivers;

namespace control
{
namespace chassis
{
class ChassisDriveCommand : public tap::control::Command
{
public:
    ChassisDriveCommand(tap::Drivers *drivers, ChassisSubsystem *sub);

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "chassis drive command"; }

private:
    tap::Drivers *drivers;
    ChassisSubsystem *chassis;
};  // class ChassisDriveCommand
}  // namespace chassis
}  // namespace control

#endif  // CHASSIS_DRIVE_COMMAND_HPP_
