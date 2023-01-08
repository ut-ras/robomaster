#include "pid_motor_controller.hpp"

#include "modm/math.hpp"

namespace pid_motor_controller
{

/// @brief Update the PID and motor drive strength
/// @param targetAngle Target angle in radians (unwrapped)
void MotorPositionController::update(float targetAngle, float dt)
{
    float diff = targetAngle - this->getAngle();
    float smallest_diff = diff - roundf(diff);
    float output = pid.update(smallest_diff, dt);
    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorPositionController::getAngle()
{
    int64_t encoderVal = motor.getEncoderUnwrapped();
    float units = static_cast<float>(encoderVal) / DjiMotor::ENC_RESOLUTION;
    float turns = units / constants.gearRatio;
    float rads = turns * M_TWOPI;
    return rads;
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