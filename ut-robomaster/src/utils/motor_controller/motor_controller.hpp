#include "tap/motor/dji_motor.hpp"

#include "motor_constants.hpp"
#include "pid.hpp"

using namespace tap::motor;

namespace motor_controller
{

class MotorController
{
public:
    MotorController(
        tap::Drivers* drivers,
        const MotorConstants& constants,
        const MotorId motorId,
        const tap::can::CanBus motorCanBus,
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
    virtual void update(float target, float dt) = 0;
    virtual float measure() = 0;

    // private:
    const MotorConstants constants;
    DjiMotor motor;
    Pid pid;
};

class MotorPositionController : public MotorController
{
public:
    using MotorController::MotorController;
    void update(float target, float dt);

    // private:
    /// @brief Get the current position of the motor.
    /// @return Motor angle, measured in revolutions.
    float measure();
};

class MotorVelocityController : public MotorController
{
public:
    using MotorController::MotorController;
    void update(float target, float dt);

    // private:
    /// @brief Get the current velocity of the motor.
    /// @return Motor velocity, measured in revolutions per second.
    float measure();
};
}  // namespace motor_controller
