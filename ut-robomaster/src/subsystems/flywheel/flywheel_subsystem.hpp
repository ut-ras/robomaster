#ifndef SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace flywheel
{

using namespace motor_controller;

class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(src::Drivers* drivers);

    ~FlywheelSubsystem() = default;

    void initialize() override;

    void refresh() override;

    void setLaunchSpeed(float speed);

    // Overloaded method to allow independent control of left and right flywheels
    void setLaunchSpeed(float leftSpeed, float rightSpeed);

private:
    src::Drivers* drivers;
    MotorVelocityController motors[FLYWHEELS];

    #if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
        float launchSpeedLeft = 0.0f;
        float launchSpeedRight = 0.0f;

    #elif defined(TARGET_HERO)
        float launchSpeedHero = 0.0f;
    #endif
};

}  // namespace shooter
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
