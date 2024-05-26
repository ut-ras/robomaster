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

class CommandMoveChassisKeyboard : public tap::control::Command
{
public:
    CommandMoveChassisKeyboard(
        src::Drivers *drivers,
        ChassisSubsystem *chassis,
        TurretSubsystem *turret,
        bool beyblade = false)
        : drivers(drivers),
          chassis(chassis),
          turret(turret),
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

    Vector2f inputMove = Vector2f(0.0f);
    const bool beyblade = false;
};
}  // namespace commands
