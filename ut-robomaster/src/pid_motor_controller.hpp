#include "tap/motor/dji_motor.hpp"

#include "pid.hpp"

using namespace tap::motor;

namespace pid_motor_controller
{

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

class MotorPositionController
{
public:
    MotorPositionController(
        tap::Drivers* drivers,
        const MotorId motorId,
        const tap::can::CanBus motorCanBus,
        const bool motorInverted,
        const char* motorName,
        const uint16_t& maxCurrent,
        const float& gearRatio,
        const float& kp = 1.0f,
        const float& ki = 0.0f,
        const float& kd = 0.0f);
    float getAngle();
    void initialize();
    void update(float targetAngle, float dt);

    // private:
    DjiMotor motor;
    const uint16_t maxCurrent;
    const float gearRatio;
    Pid pid;
};
}  // namespace pid_motor_controller
