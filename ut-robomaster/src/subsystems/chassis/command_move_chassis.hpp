#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"
#include "utils/chassis_auto_align.hpp"

#include "drivers.hpp"

namespace commands
{
using namespace tap::communication::serial;
using namespace modm;
using subsystems::chassis::ChassisSubsystem;
using subsystems::turret::TurretSubsystem;

class CommandMoveChassis : public tap::control::Command
{
public:
    CommandMoveChassis(
        src::Drivers *drivers,
        ChassisSubsystem *chassis,
        TurretSubsystem *turret,
        bool turretRelative = false,
        bool beyblade = false)
        : drivers(drivers),
          chassis(chassis),
          turret(turret),
          turretRelative(turretRelative),
          beyblade(beyblade)
    {
        addSubsystemRequirement(chassis);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "move chassis command"; }

private:
    src::Drivers *drivers;
    ChassisSubsystem *chassis;
    TurretSubsystem *turret;

    Vector2f keyboardInputMove = Vector2f(0.0f);
    const bool turretRelative = false;
    const bool beyblade = false;

    bool applyKeyboardInput(Vector2f &moveOut, float &spinOut);
    bool applyJoystickInput(Vector2f &moveOut, float &spinOut);
};
}  // namespace commands
