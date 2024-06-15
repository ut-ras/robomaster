#pragma once

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/control/subsystem.hpp"

#include "robots/robot_constants.hpp"
#include "subsystems/flywheel/flywheel_subsystem.hpp"
#include "utils/motors/motor_controller.hpp"

#include "drivers.hpp"

namespace subsystems
{
namespace agitator
{
using flywheel::FlywheelSubsystem;
using motors::MotorController;

class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    AgitatorSubsystem(src::Drivers *drivers, FlywheelSubsystem *flywheel, MotorConfig motor);

    ~AgitatorSubsystem() = default;

    void initialize() override;

    void refresh() override;

    float getShapedVelocity(float time, float a, float phi, float ballsPerSecond);
    void setBallsPerSecond(float bps);

    float getPosition();
    float getVelocity();

private:
    src::Drivers *drivers;
    FlywheelSubsystem *flywheel;

    float ballsPerSecond = 0.0f;

    MotorController agitator;
#ifdef TARGET_HERO
    MotorController feeder;
#endif
};

}  // namespace agitator
}  // namespace subsystems
