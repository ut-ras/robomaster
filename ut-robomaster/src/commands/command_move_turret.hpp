#pragma once

#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"
#include "robots/robot_state.hpp"
#include "subsystems/turret/turret_subsystem.hpp"

#include "drivers.hpp"

namespace commands
{
using subsystems::turret::TurretSubsystem;
using tap::communication::serial::Remote;

class CommandMoveTurret : public tap::control::Command
{
public:
    CommandMoveTurret(src::Drivers* drivers, RobotState* state, TurretSubsystem* turret)
        : drivers(drivers),
          state(state),
          turret(turret)
    {
        addSubsystemRequirement(turret);
    }

    void initialize() override;
    void execute() override;
    void end(bool interrupted) override;
    bool isFinished() const override;
    const char* getName() const override { return "move turret command"; }

private:
    src::Drivers* drivers;
    RobotState* state;
    TurretSubsystem* turret;

    static constexpr float MOUSE_SENS_YAW = 0.01f;
    static constexpr float MOUSE_SENS_PITCH = 0.01f;

    float yaw = 0.0f;
    float pitch = 0.0f;

#if defined TARGET_STANDARD || defined TARGET_SENTRY
    static constexpr float yawInputScale = 0.02f;
    static constexpr float pitchInputScale = 0.01f;
#elif defined TARGET_HERO
    static constexpr float yawInputScale = 0.008f;
    static constexpr float pitchInputScale = 0.01f;
#endif
};
}  // namespace commands