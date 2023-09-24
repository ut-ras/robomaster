#pragma once

#include "tap/control/command.hpp"

#include "robots/robot_state.hpp"
#include "subsystems/chassis/chassis_subsystem.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using namespace tap::communication::serial;
using namespace modm;
using subsystems::chassis::ChassisSubsystem;
using subsystems::turret::TurretSubsystem;

class CommandBeybladeChassisKeyboard : public tap::control::Command
{
public:
    CommandBeybladeChassisKeyboard(
        src::Drivers *drivers,
        ChassisSubsystem *chassis,
        TurretSubsystem *turret)
        : drivers(drivers),
          chassis(chassis),
          turret(turret)
    {
        addSubsystemRequirement(chassis);
    }

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char *getName() const override { return "move chassis command"; }

private:
    static constexpr float DELTA_TIME = 0.002f;
    static constexpr float ANALOG_DEAD_ZONE = 0.1f;
    static constexpr float TURRET_ALIGN_FACTOR = 0.5f;
    static constexpr float KEYBOARD_ACCEL = 5.0f;
    static constexpr float KEYBOARD_DECEL = 5.0f;
    static constexpr float SNAP_ANGLE = M_PI;
    static constexpr float TRANSLATION_RATIO = 0.5f;

    src::Drivers *drivers;
    ChassisSubsystem *chassis;
    TurretSubsystem *turret;

    Vector2f inputMove = Vector2f(0.0f);
    float inputSpin = 0.0f;
};
}  // namespace commands
