#pragma once

// #include "tap/architecture/timeout.hpp"
#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
// this is older sentry code dont know why it had chopped path
// #include "chassis_subsystem.hpp"
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
using tap::arch::MilliTimeout;

class CommandSentryPosition : public tap::control::Command
{
public:
    CommandSentryPosition(
        src::Drivers* drivers,
        ChassisSubsystem* chassis,
        TurretSubsystem* turret,
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

    const char* getName() const override { return "sentry position command"; }

private:
    src::Drivers* drivers;
    ChassisSubsystem* chassis;
    MilliTimeout moveTimer;
    TurretSubsystem* turret;

    Vector2f keyboardInputMove = Vector2f(0.0f);
    const bool turretRelative = false;
    const bool beyblade = false;

    bool applyKeyboardInput(Vector2f& moveOut, float& spinOut);
    bool applyJoystickInput(Vector2f& moveOut, float& spinOut);
};
}  // namespace commands