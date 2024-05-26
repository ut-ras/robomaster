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

class CommandMoveTurretJoystick : public tap::control::Command
{
public:
    CommandMoveTurretJoystick(src::Drivers* drivers, TurretSubsystem* turret)
        : drivers(drivers),
          turret(turret)
    {
        addSubsystemRequirement(turret);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char* getName() const override { return "move turret joystick command"; }

private:
    src::Drivers* drivers;
    TurretSubsystem* turret;

    bool isCalibrated = false;

    float yaw = 0.0f;
    float pitch = 0.0f;
};
}  // namespace commands