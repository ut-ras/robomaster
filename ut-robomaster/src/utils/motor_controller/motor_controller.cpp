#include "motor_controller.hpp"

#include "modm/math.hpp"

namespace motor_controller
{
float MotorController::delta_time()
{
    uint32_t time = tap::arch::clock::getTimeMilliseconds();
    float dt = (time - lastTime) / 1000.0f;
    lastTime = time;
    return dt;
}

// Position
void MotorPositionController::update(float target)
{
    float diff = target - this->measure();
    float smallest_diff = diff - roundf(diff);
    float output = pid.update(smallest_diff, delta_time());
    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorPositionController::measure()
{
    int64_t encoderVal = motor.getEncoderUnwrapped();
    float units = static_cast<float>(encoderVal) / DjiMotor::ENC_RESOLUTION;
    float turns = units / constants.gearRatio;
    return turns;
}

// Velocity
void MotorVelocityController::update(float target)
{
    float diff = target - this->measure();
    float output = pid.update(diff, delta_time());
    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorVelocityController::measure()
{
    int16_t rpm = motor.getShaftRPM() / constants.gearRatio;
    float rps = rpm / 60.0f;  // revs / sec
    return rps;
}
}  // namespace motor_controller