#pragma once

#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::turret::TurretSubsystem;
using tap::communication::serial::Remote;

class CommandSentryAim : public tap::control::Command
{
public:
    CommandSentryAim(src::Drivers* drivers, TurretSubsystem* turret)
        : drivers(drivers),
          turret(turret)
    {
        addSubsystemRequirement(turret);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char* getName() const override { return "aim sentry command"; }

private:
    src::Drivers* drivers;
    TurretSubsystem* turret;
};
}  // namespace commands
