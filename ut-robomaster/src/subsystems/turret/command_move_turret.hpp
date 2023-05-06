#ifndef COMMAND_MOVE_TURRET_HPP_
#define COMMAND_MOVE_TURRET_HPP_

#include "tap/communication/serial/remote.hpp"
#include "tap/control/command.hpp"

#include "robots/robot_constants.hpp"

#include "drivers.hpp"
#include "turret_subsystem.hpp"

using tap::communication::serial::Remote;
using tap::control::Subsystem;

namespace subsystems
{
namespace turret
{
class CommandMoveTurret : public tap::control::Command
{
public:
    CommandMoveTurret(src::Drivers* drivers, TurretSubsystem* turret)
        : drivers(drivers),
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
    TurretSubsystem* turret;

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
}  // namespace turret
}  // namespace subsystems

#endif
