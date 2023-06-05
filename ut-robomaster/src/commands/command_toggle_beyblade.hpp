#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_state.hpp"

#include "drivers.hpp"

namespace commands
{
using namespace tap::communication::serial;

class CommandToggleBeyblade : public tap::control::Command
{
public:
    CommandToggleBeyblade(src::Drivers *drivers, RobotState *state) : drivers(drivers), state(state)
    {
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "toggle beyblade command"; }

private:
    src::Drivers *drivers;
    RobotState *state;
};
}  // namespace commands
