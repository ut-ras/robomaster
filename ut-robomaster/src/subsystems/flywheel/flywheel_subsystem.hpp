#ifndef SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "utils/motors/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace flywheel
{

using motors::MotorController;

class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(src::Drivers* drivers);

    ~FlywheelSubsystem() = default;

    void initialize() override;

    void refresh() override;

    /// @brief Change flywheel velocity.
    /// @param velocity Velocity in rev/s.
    void setVelocity(float velocity);

    bool isActive();

private:
    src::Drivers* drivers;
    MotorController motors[FLYWHEELS];
    float velocity = 0.0f;
};

}  // namespace flywheel
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
