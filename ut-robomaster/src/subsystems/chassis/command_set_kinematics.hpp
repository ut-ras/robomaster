#include "tap/control/command.hpp"

#include "chassis_subsystem.hpp"

namespace subsystems
{
namespace chassis
{
class SetKinematicsCommand : public tap::control::Command
{
public:
    SetKinematicsCommand(ChassisSubsystem *sub) : subsystem(sub) { addSubsystemRequirement(sub); }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "set kinematics command"; }

private:
    ChassisSubsystem *subsystem;
};
}  // namespace chassis
}  // namespace subsystems