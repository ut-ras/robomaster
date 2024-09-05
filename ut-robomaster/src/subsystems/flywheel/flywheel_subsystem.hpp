#pragma once

#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "utils/motors/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems::flywheel
{
class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(src::Drivers* drivers);

    void initialize() override;

    void refresh() override;

    /// @brief Change flywheel velocity.
    /// @param velocity Velocity in rev/s.
    void setVelocity(float velocity);

    bool isActive();

private:
    src::Drivers* drivers;

    motors::MotorController motor_L;
    motors::MotorController motor_R;

    float velocity = 0.0f;
};
}  // namespace subsystems::flywheel