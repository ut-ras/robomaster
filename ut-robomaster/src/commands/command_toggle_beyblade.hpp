#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_state.hpp"

#include "drivers.hpp"

namespace commands
{
class CommandToggleBeyblade : public tap::control::Command
{
public:
    CommandToggleBeyblade(RobotState *state) : state(state) {}

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char *getName() const override { return "toggle beyblade command"; }

private:
    RobotState *state;
};
}  // namespace commands
