#ifndef SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
#define SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "utils/motor_controller/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace agitator
{
using namespace motor_controller;

struct TurretCooldownPercentage
{
    int currentHeat1;
    int currentHeat2;
    int heatLimit1;
    int heatLimit2;
    int percentage1;
    int percentage2;

    bool compare()
    {
        percentage1 = currentHeat1 / heatLimit1;
        percentage2 = currentHeat2 / heatLimit2;
        return percentage1 - percentage2;  // if positive, turret one has a higher cooldown
                                           // if zero, both turrets have equivalent cooldown
                                           // if negative, percentage 2 has a higher cooldown
    }
};

// Agitator uses DJI motors
class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    AgitatorSubsystem(src::Drivers *drivers);

    ~AgitatorSubsystem() = default;

    void initialize() override;

    void refresh() override;

    float getShapedVelocity(float time, float a, float phi, float ballsPerSecond);

    void setBallsPerSecond(float bps);

    // Overloaded method to allow independent control of left and right agitators
    void setBallsPerSecond(float bpsLeft, float bpsRight);

private:
    src::Drivers *drivers;

#if defined(TARGET_STANDARD) || defined(TARGET_SENTRY)
    MotorVelocityController leftAgitator;
    MotorVelocityController rightAgitator;
    TurretCooldownPercentage cooldownMeter;

    float ballsPerSecondLeft = 0.0f;
    float ballsPerSecondRight = 0.0f;

#elif defined(TARGET_HERO)
    MotorVelocityController agitator;
    MotorVelocityController feeder;

    float ballsPerSecondHero = 0.0f;
#endif

    float startTime = 0.0f;
};

}  // namespace agitator
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
