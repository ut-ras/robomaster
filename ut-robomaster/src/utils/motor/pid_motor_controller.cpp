#include "pid_motor_controller.hpp"

#include "modm/math.hpp"

namespace pid_motor_controller
{
void MotorPositionController::update(float target, float dt)
{
    float diff = target - this->measure();
    float smallest_diff = diff - roundf(diff);
    float output = pid.update(smallest_diff, dt);
    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorPositionController::measure()
{
    int64_t encoderVal = motor.getEncoderUnwrapped();
    float units = static_cast<float>(encoderVal) / DjiMotor::ENC_RESOLUTION;
    float turns = units / constants.gearRatio;
    return turns;
}

void MotorVelocityController::update(float target, float dt)
{
    float diff = target - this->measure();
    float output = pid.update(diff, dt);
    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorVelocityController::measure()
{
    int16_t rpm = motor.getShaftRPM() / constants.gearRatio;
    float rps = rpm / 60.0f;  // revs / sec
    return rps;
}
}  // namespace pid_motor_controller