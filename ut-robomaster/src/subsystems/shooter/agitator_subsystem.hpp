#ifndef SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_

#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace shooter
{
using namespace motor_controller;

// Agitator uses DJI motors
class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    AgitatorSubsystem(src::Drivers *drivers);
    ~AgitatorSubsystem() = default;

    void initialize() override;
    void refresh() override;
    void setShooting(bool shooting);

private:
    const float SPEED = 0.5f;

    src::Drivers *drivers;

#if defined TARGET_STANDARD || defined TARGET_SENTRY
    MotorVelocityController leftMotor;
    MotorVelocityController rightMotor;
#elif defined TARGET_HERO
    MotorVelocityController motor;
#endif

    bool isShooting = false;
};

}  // namespace shooter
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
