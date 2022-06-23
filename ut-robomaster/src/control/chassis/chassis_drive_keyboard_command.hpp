#ifndef CHASSIS_DRIVE_KEYBOARD_COMMAND_HPP_
#define CHASSIS_DRIVE_KEYBOARD_COMMAND_HPP_

#include "tap/control/command.hpp"
#include "tap/algorithms/ramp.hpp"

#include "chassis_subsystem.hpp"

class Drivers;

namespace control
{
namespace chassis
{
class ChassisDriveKeyboardCommand : public tap::control::Command
{
public:
    ChassisDriveKeyboardCommand(tap::Drivers *drivers, ChassisSubsystem *sub);

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "chassis drive keyboard"; }

private:
    tap::Drivers *drivers;
    ChassisSubsystem *chassis;
    tap::algorithms::Ramp xRamp;
    tap::algorithms::Ramp yRamp;
    tap::algorithms::Ramp rRamp;

    bool xToggle;
    bool yToggle;
    bool rToggle;
    bool xPositive;
    bool yPositive;
    bool rPositive;
    bool xDeacceleration;
    bool yDeacceleration;
    bool rDeacceleration;

};  // class ChassisDriveKeyboardCommand
}  // namespace chassis
}  // namespace control

#endif  // CHASSIS_DRIVE_KEYBOARD_COMMAND_HPP_
