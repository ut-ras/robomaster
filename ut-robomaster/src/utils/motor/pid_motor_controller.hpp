#include "tap/motor/dji_motor.hpp"

#include "pid.hpp"

using namespace tap::motor;

namespace pid_motor_controller
{

class MotorController
{
public:
    MotorController(
        tap::Drivers* drivers,
        const MotorId motorId,
        const tap::can::CanBus motorCanBus,
        const bool motorInverted,
        const char* motorName,
        const uint16_t& maxCurrent,
        const float& gearRatio,
        const float& kp = 1.0f,
        const float& ki = 0.0f,
        const float& kd = 0.0f)
        : motor(drivers, motorId, motorCanBus, motorInverted, motorName),
          maxCurrent(maxCurrent),
          gearRatio(gearRatio),
          pid(kp, ki, kd)
    {
    }

    void initialize() { motor.initialize(); }
    virtual void update(float target, float dt) = 0;

    // private:
    DjiMotor motor;
    const uint16_t maxCurrent;
    const float gearRatio;
    Pid pid;
};

// class MotorVelocityController
// {
// public:
//     MotorVelocityController(
//         tap::motor::DjiMotor* motor,
//         const float& kp = 5.0f,
//         const float& ki = 0.0f,
//         const float& kd = 1.0f,
//         const float& maxErrorSum = 0.0f,
//         const float& maxOutput = 16000.0f);
//     void update(float targetVelocity);

// private:
//     tap::motor::DjiMotor* motor;
//     modm::Pid<float> pid;
// };

class MotorPositionController : public MotorController
{
public:
    using MotorController::MotorController;
    void update(float target, float dt);

    // private:
    float getAngle();
};
}  // namespace pid_motor_controller
