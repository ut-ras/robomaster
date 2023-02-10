#include "tap/motor/dji_motor.hpp"

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
        const float& kp = 1.0f,
        const float& ki = 0.0f,
        const float& kd = 0.0f)
        : constants(constants),
          motor(drivers, motorId, motorCanBus, motorInverted, motorName),
          pid(kp, ki, kd)
    {
    }

    void initialize() { motor.initialize(); }
    virtual void update(float target) = 0;
    virtual float measure() = 0;

protected:
    float delta_time();
    uint32_t lastTime = 0;
    const MotorConstants constants;
    DjiMotor motor;
    Pid pid;
};

class MotorPositionController : public MotorController
{
public:
    using MotorController::MotorController;

    /// @brief Update the controller with the desired target position.
    /// @return Target angle, measured in revolutions.
    void update(float target);

    /// @brief Get the current position of the motor.
    /// @return Motor angle, measured in revolutions.
    float measure();
};

class MotorVelocityController : public MotorController
{
public:
    using MotorController::MotorController;

    /// @brief Update the controller with the desired target velocity.
    /// @return Target velocity, measured in revolutions per second.
    void update(float target);

    /// @brief Get the current velocity of the motor.
    /// @return Motor velocity, measured in revolutions per second.
    float measure();
};
}  // namespace motor_controller
