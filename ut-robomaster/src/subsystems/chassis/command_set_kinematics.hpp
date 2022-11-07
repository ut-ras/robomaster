#include "tap/control/command.hpp"

#include "chassis_subsystem.hpp"
#include "drivers.hpp"

namespace subsystems
{
namespace chassis
{
class SetKinematicsCommand : public tap::control::Command
{
public:
    SetKinematicsCommand(ChassisSubsystem *sub, tap::Drivers *drivers)
        : drivers(drivers),
          subsystem(sub),
          isRKeyPressed(false)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "set kinematics command"; }

private:
    tap::Drivers *drivers;
    ChassisSubsystem *subsystem;
    bool isRKeyPressed;
};
}  // namespace chassis
}  // namespace subsystems