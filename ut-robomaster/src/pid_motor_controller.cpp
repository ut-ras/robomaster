#include "pid_motor_controller.hpp"

#include "modm/math.hpp"

#define DELTA_TIME 0.002f

namespace pid_motor_controller
{
MotorPositionController::MotorPositionController(
    DjiMotor motor,
    const uint16_t& maxCurrent,
    const float& kp,
    const float& ki,
    const float& kd)
    : motor(&motor),
      maxCurrent(maxCurrent),
      pid(kp, ki, kd)
{
}

void MotorPositionController::init() { motor->initialize(); }

/// @brief Update the PID and motor drive strength
/// @param targetAngle Target angle in radians (unwrapped)
void MotorPositionController::update(float targetAngle)
{
    float output = pid.update(targetAngle, this->getAngle(), DELTA_TIME);
    motor->setDesiredOutput(output * maxCurrent);
}

float MotorPositionController::getAngle()
{
    int64_t encoderVal = motor->getEncoderUnwrapped();
    float unwrappedAngle = (M_TWOPI * encoderVal) / DjiMotor::ENC_RESOLUTION;
    return unwrappedAngle;
}

// MotorVelocityController::MotorVelocityController(
//     DjiMotor* motor,
//     const float& kp,
//     const float& ki,
//     const float& kd,
//     const float& maxErrorSum,
//     const float& maxOutput)
//     : motor(motor),
//       pid(kp, ki, kd, maxErrorSum, maxOutput)
// {
// }

// void MotorVelocityController::update(float targetVelocity)
// {
//     float error = targetVelocity - motor->getShaftRPM();
//     pid->update(error);
// }
}  // namespace pid_motor_controller