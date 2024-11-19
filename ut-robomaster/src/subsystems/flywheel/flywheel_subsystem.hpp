#pragma once

#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/subsystem.hpp"
#include "utils/motors/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems::flywheel
{
using motors::MotorController;

class FlywheelSubsystem : public Subsystem
{
public:
    FlywheelSubsystem(src::Drivers* drivers);
    void initialize() override;
    void refresh() override;

    /// @brief Change flywheel velocity.
    /// @param velocity Velocity in rev/s.
    void setVelocity(float velocity);

    bool isActive();

    bool hardwareOk() override;

private:
    src::Drivers* drivers;
    MotorController motors[FLYWHEELS];
    float velocity = 0.0f;
};
}  // namespace subsystems::flywheel
