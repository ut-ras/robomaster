#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using namespace tap::communication::serial;
using namespace modm;
using subsystems::chassis::ChassisSubsystem;
using subsystems::turret::TurretSubsystem;

class CommandMoveChassisJoystick : public tap::control::Command
{
public:
    CommandMoveChassisJoystick(
        src::Drivers *drivers,
        ChassisSubsystem *chassis,
        TurretSubsystem *turret,
        bool turretRelative = false)
        : drivers(drivers),
          chassis(chassis),
          turret(turret),
          turretRelative(turretRelative)
    {
        addSubsystemRequirement(chassis);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "move chassis joystick command"; }

private:
    src::Drivers *drivers;
    ChassisSubsystem *chassis;
    TurretSubsystem *turret;

    const bool turretRelative = false;
};
}  // namespace commands
