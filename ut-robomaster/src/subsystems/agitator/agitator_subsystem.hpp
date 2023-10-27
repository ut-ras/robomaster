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

// Agitator uses DJI motors
class AgitatorSubsystem : public tap::control::Subsystem
{
public:
    AgitatorSubsystem(src::Drivers *drivers);

    ~AgitatorSubsystem() = default;

    void initialize() override;

    void refresh() override;

    void setSpeed(float newSpeed);

private:
    src::Drivers *drivers;
    MotorVelocityController agitator1;
    MotorVelocityController agitator2;
    float speed = 0.0f;
};

}  // namespace agitator
}  // namespace subsystems

#endif  // SUBSYSTEMS_SHOOTER_AGITATOR_SUBSYSTEM_HPP_
