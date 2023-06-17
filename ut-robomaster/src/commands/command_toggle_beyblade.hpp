#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_state.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::chassis::ChassisSubsystem;

class CommandToggleBeyblade : public tap::control::Command
{
public:
    CommandToggleBeyblade(ChassisSubsystem *chassis, RobotState *state) : state(state)
    {
        addSubsystemRequirement(chassis);
    }

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char *getName() const override { return "toggle beyblade command"; }

private:
    RobotState *state;
};
}  // namespace commands
