#include "motor_controller.hpp"

namespace
{
using namespace motor_controller;
using namespace tap::algorithms;

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

void MotorController::initialize()
{
    motor.initialize();
    setActive(true);
}

void MotorController::setActive(bool active)
{
    isActive = active;
    if (!active)
    {
        pid.reset();
        motor.setDesiredOutput(0);
    }
}

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
    if (!isActive) return;

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

void MotorPositionController::setActive(bool active)
{
    MotorController::setActive(active);
    if (!active) velocityPid.reset();
}

// Velocity
void MotorVelocityController::update(float target)
{
    if (!isActive) return;

    float diff = target - this->measure();
    float output = pid.update(diff, delta_time());
    motor.setDesiredOutput(output * constants.maxOutput);
}

// Overload method to allow for power limiting
// Non-overloaded update method should be called first before this one, as this does not update the
// pid
void MotorVelocityController::applyPowerScalar(float powerLimitScalar)
{
    if (!isActive) return;
    motor.setDesiredOutput(motor.getOutputDesired() * powerLimitScalar);
}

float MotorVelocityController::measure() { return measure_velocity(&motor, constants); }
}  // namespace motor_controller