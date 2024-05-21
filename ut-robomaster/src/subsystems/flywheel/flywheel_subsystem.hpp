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

using namespace motors;

class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(src::Drivers* drivers);

    ~FlywheelSubsystem() = default;

    void initialize() override;

    void refresh() override;

    void setLaunchSpeed(float speed);

private:
    src::Drivers* drivers;
    MotorVelocityController motors[FLYWHEELS];
    float launchSpeed = 0.0f;
};

}  // namespace flywheel
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
