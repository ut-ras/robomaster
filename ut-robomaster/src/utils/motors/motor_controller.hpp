#ifndef MOTOR_CONTROLLER_HPP_
#define MOTOR_CONTROLLER_HPP_

#include "tap/motor/dji_motor.hpp"

#include "drivers.hpp"
#include "motor_constants.hpp"
#include "pid.hpp"

using tap::can::CanBus;
using tap::motor::DjiMotor;
using tap::motor::MotorId;

namespace motors
{
using motor_controller::Pid;
using motor_controller::PidConstants;

class MotorController
{
public:
    MotorController(src::Drivers* drivers, const MotorConfig motor)
        : constants(motor.constants),
          motor(drivers, motor.id, motor.canBus, motor.inverted, motor.name),
          velocityPid(motor.velocityPidConstants),
          positionPid(motor.positionPidConstants)
    {
    }

    void initialize();
    void setActive(bool active);
    void updatePosition(float target);
    void updateVelocity(float target);
    void setOutput(float output);
    bool isOnline();

    /// @brief Get the current position of the motor.
    /// @return Angle, measured in revolutions.
    float measurePosition();

    /// @brief Get the current velocity of the motor.
    /// @return Angular velocity, measured in revolutions per second.
    float measureVelocity();

    void applyPowerScalar(float powerLimitScalar);

protected:
    uint32_t lastTime = 0;
    bool isActive = false;
    const MotorSpecs constants;
    DjiMotor motor;
    Pid velocityPid;
    Pid positionPid;
};
}  // namespace motors

#endif