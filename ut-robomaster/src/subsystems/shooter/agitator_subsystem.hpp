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
    src::Drivers *drivers;

#if defined TARGET_STANDARD || defined TARGET_SENTRY
    const float BALLS_PER_SEC = 4.0f;
    const float BALLS_PER_REV = 8.0f;

    MotorVelocityController leftAgitator;
    MotorVelocityController rightAgitator;
#elif defined TARGET_HERO
    const float BALLS_PER_SEC = 2.0f;
    const float BALLS_PER_REV = 6.0f;
    const float FEEDER_SPEED = BALLS_PER_SEC / BALLS_PER_REV;

    MotorVelocityController agitator;
    MotorVelocityController feeder;
#endif

    float startTime = 0.0f;
    bool isShooting = false;
};

}  // namespace shooter
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
