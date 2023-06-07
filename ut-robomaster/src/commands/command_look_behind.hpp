#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_state.hpp"

#include "drivers.hpp"

namespace commands
{
class CommandLookBehind : public tap::control::Command
{
public:
    CommandLookBehind(RobotState *state) : state(state) {}

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char *getName() const override { return "look behind command"; }

private:
    RobotState *state;
};
}  // namespace commands
