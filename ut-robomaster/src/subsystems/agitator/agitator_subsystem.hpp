#pragma once

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
using tap::motor::MotorId;

class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    AgitatorSubsystem(src::Drivers *drivers, MotorId agitatorMotorId, bool motorFlipped);

    ~AgitatorSubsystem() = default;

    void initialize() override;

    void refresh() override;

    float getShapedVelocity(float time, float a, float phi, float ballsPerSecond);
    void setBallsPerSecond(float bps);

    float getPosition();

private:
    src::Drivers *drivers;

    float ballsPerSecond = 0.0f;

    MotorVelocityController agitator;
#ifdef TARGET_HERO
    MotorVelocityController feeder;
#endif
};

}  // namespace agitator
}  // namespace subsystems
