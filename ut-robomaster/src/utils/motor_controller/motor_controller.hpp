#ifndef MOTOR_CONTROLLER_HPP_
#define MOTOR_CONTROLLER_HPP_

#include "tap/motor/dji_motor.hpp"

#include "drivers.hpp"
#include "motor_constants.hpp"
#include "pid.hpp"

using tap::can::CanBus;
using tap::motor::DjiMotor;
using tap::motor::MotorId;

namespace motor_controller
{
class MotorController
{
public:
    MotorController(
        src::Drivers* drivers,
        const MotorConstants& constants,
        const MotorId motorId,
        const CanBus motorCanBus,
        const bool motorInverted,
        const char* motorName,
        const PidConstants& pidConstants)
        : constants(constants),
          motor(drivers, motorId, motorCanBus, motorInverted, motorName),
          pid(pidConstants)
    {
    }

    void initialize();
    virtual void setActive(bool active);
    virtual void update(float target) = 0;
    virtual Pid getPid() { return pid; }

    /// @brief Get the current position of the motor.
    /// @return Angle, measured in revolutions.
    float measurePosition();

    /// @brief Get the current velocity of the motor.
    /// @return Angular velocity, measured in revolutions per second.
    float measureVelocity();

protected:
    float deltaTime();
    uint32_t lastTime = 0;
    bool isActive = false;
    const MotorConstants constants;
    DjiMotor motor;
    Pid pid;
};

class MotorPositionController : public MotorController
{
public:
    MotorPositionController(
        src::Drivers* drivers,
        const MotorConstants& constants,
        const MotorId motorId,
        const CanBus motorCanBus,
        const bool motorInverted,
        const char* motorName,
        const PidConstants& pidConstantsVel,
        const PidConstants& pidConstantsPos)
        : MotorController(
              drivers,
              constants,
              motorId,
              motorCanBus,
              motorInverted,
              motorName,
              pidConstantsPos),
          velocityPid(pidConstantsVel)
    {
    }

    /// @brief Update the controller with the desired target position.
    /// @return Target angle, measured in revolutions.
    void update(float target);

    void setActive(bool active) override;

private:
    Pid velocityPid;
};

class MotorVelocityController : public MotorController
{
public:
    using MotorController::MotorController;

    /// @brief Update the controller with the desired target velocity.
    /// @return Target velocity, measured in revolutions per second.
    void update(float target);

    void applyPowerScalar(float powerLimitScalar);
};
}  // namespace motor_controller

#endif