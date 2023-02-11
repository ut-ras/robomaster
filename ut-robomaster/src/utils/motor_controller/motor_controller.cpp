#include "motor_controller.hpp"

namespace
{
using namespace motor_controller;

float measure_position(DjiMotor* motor, const MotorConstants& constants)
{
    int64_t encoderVal = motor->getEncoderUnwrapped();
    float units = static_cast<float>(encoderVal) / DjiMotor::ENC_RESOLUTION;
    float turns = units / constants.gearRatio;  // revs
    return turns;
}

float measure_velocity(DjiMotor* motor, const MotorConstants& constants)
{
    int16_t rpm = motor->getShaftRPM() / constants.gearRatio;
    float rps = rpm / 60.0f;  // revs / sec
    return rps;
}
}  // namespace

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
    float dt = delta_time();

    // position
    float diff = target - this->measure();
    float smallest_diff = diff - roundf(diff);
    float velocity = pid.update(smallest_diff, dt);

    // velocity
    float vel_diff = velocity - measure_velocity(&motor, constants);
    float output = velocityPid.update(vel_diff, dt);

    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorPositionController::measure() { return measure_position(&motor, constants); }

// Velocity
void MotorVelocityController::update(float target)
{
    float diff = target - this->measure();
    float output = pid.update(diff, delta_time());
    motor.setDesiredOutput(output * constants.maxOutput);
}

float MotorVelocityController::measure() { return measure_velocity(&motor, constants); }
}  // namespace motor_controller