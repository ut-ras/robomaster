#ifndef COMMAND_SENTRY_POSITION_HPP_
#define COMMAND_SENTRY_POSITION_HPP_

#include "tap/control/command.hpp"

#include "chassis_subsystem.hpp"
#include "drivers.hpp"

namespace commands
{
using namespace tap::communication::serial;
using namespace modm;
using subsystems::chassis::ChassisSubsystem;
using tap::arch::clock::getTimeMilliseconds;

class CommandSentryPosition : public tap::control::Command
{
public:
    CommandSentryPosition(src::Drivers *drivers, ChassisSubsystem *chassis)
        : drivers(drivers),
          chassis(chassis)
    {
        addSubsystemRequirement(chassis);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "sentry position command"; }

private:
    src::Drivers *drivers;
    ChassisSubsystem *chassis;
    // bool isStarted = false;
    // float startTime = 0.0f;
    float startup_time = 0.0f;
};
}  // namespace commands

#endif
