#pragma once

#include "tap/communication/sensors/buzzer/buzzer.hpp"
#include "tap/control/command.hpp"

#include "drivers.hpp"
#include "startup_subsystem.hpp"

namespace commands
{

using subsystems::startup::StartupSubsystem;

class CommandStartup : public tap::control::Command
{
public:
    CommandStartup(src::Drivers *drivers, StartupSubsystem *startup)
        : drivers(drivers),
          startup(startup)
    {
        addSubsystemRequirement(startup);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "startup command"; }

private:
    src::Drivers *drivers;
    StartupSubsystem *startup;

};  // class CommandStartup
}  // namespace commands