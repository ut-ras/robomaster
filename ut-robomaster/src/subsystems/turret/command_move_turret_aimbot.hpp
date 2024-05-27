#pragma once

#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::turret::TurretSubsystem;
using tap::communication::serial::Remote;

class CommandMoveTurretAimbot : public tap::control::Command
{
public:
    CommandMoveTurretAimbot(src::Drivers* drivers, TurretSubsystem* turret)
        : drivers(drivers),
          turret(turret)
    {
        addSubsystemRequirement(turret);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char* getName() const override { return "move turret aimbot command"; }

private:
    src::Drivers* drivers;
    TurretSubsystem* turret;

    uint8_t lastTurretDataIndex = 0;
};
}  // namespace commands