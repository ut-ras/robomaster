#include "tap/control/command.hpp"

#include "chassis_subsystem.hpp"
#include "drivers.hpp"

namespace subsystems
{
namespace chassis
{
class SetKinematicsKeyboardCommand : public tap::control::Command
{
public:
    SetKinematicsKeyboardCommand(ChassisSubsystem *sub, tap::Drivers *drivers)
        : drivers(drivers),
          subsystem(sub)
    {
        addSubsystemRequirement(sub);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "Set kinematics command for keyboard"; }

private:
    tap::Drivers *drivers;
    ChassisSubsystem *subsystem;
};
}  // namespace chassis
}  // namespace subsystems