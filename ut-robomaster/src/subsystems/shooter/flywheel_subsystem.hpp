#ifndef SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace shooter
{

using namespace motor_controller;

class FlywheelSubsystem : public tap::control::Subsystem
{
public:
    FlywheelSubsystem(src::Drivers* drivers);
    ~FlywheelSubsystem() = default;
    void initialize() override;
    void refresh() override;

    void setActive(bool active);

private:
#if defined TARGET_STANDARD || defined TARGET_SENTRY
    static constexpr int FLYWHEELS = 4;
    static constexpr float SPEED = 80.0f;
#elif defined TARGET_HERO
    static constexpr int FLYWHEELS = 2;
    static constexpr float SPEED = 120.0f;
#endif
    src::Drivers* drivers;

    MotorVelocityController motors[FLYWHEELS];

    bool isActive = false;
};

}  // namespace shooter
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_FLYWHEEL_SUBSYSTEM_HPP_
