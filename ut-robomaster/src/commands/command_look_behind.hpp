#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_state.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::turret::TurretSubsystem;

class CommandLookBehind : public tap::control::Command
{
public:
    CommandLookBehind(TurretSubsystem *turret, RobotState *state) : state(state)
    {
        addSubsystemRequirement(turret);
    }

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char *getName() const override { return "look behind command"; }

private:
    RobotState *state;
};
}  // namespace commands
