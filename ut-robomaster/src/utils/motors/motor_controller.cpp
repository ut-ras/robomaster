#include "motor_controller.hpp"

namespace motors
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

float MotorController::measurePosition()
{
    int64_t encoderVal = motor.getEncoderUnwrapped();
    float units = static_cast<float>(encoderVal) / DjiMotor::ENC_RESOLUTION;
    float turns = units / constants.gearRatio;  // revs
    return turns;
}

float MotorController::measureVelocity()
{
    int16_t rpm = motor.getShaftRPM() / constants.gearRatio;
    float rps = rpm / 60.0f;  // revs / sec
    return rps;
}

float MotorController::deltaTime()
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

    float dt = deltaTime();

    // position
    float diff = target - measurePosition();
    float smallest_diff = diff - roundf(diff);
    float velocity = pid.update(smallest_diff, dt);

    // velocity
    float vel_diff = velocity - measureVelocity();
    float output = velocityPid.update(vel_diff, dt);

    motor.setDesiredOutput(output * constants.maxOutput);
}

void MotorPositionController::setActive(bool active)
{
    MotorController::setActive(active);
    if (!active) velocityPid.reset();
}

// Velocity
void MotorVelocityController::update(float target)
{
    if (!isActive) return;

    float diff = target - measureVelocity();
    float output = pid.update(diff, deltaTime());
    motor.setDesiredOutput(output * constants.maxOutput);
}

// Overload method to allow for power limiting
// Non-overloaded update method should be called first before this one, as this does not update the
// pid
void MotorVelocityController::applyPowerScalar(float powerLimitScalar)
{
    if (!isActive) return;
    float invertFactor = motor.isMotorInverted() ? -1.0f : 1.0f;
    motor.setDesiredOutput(motor.getOutputDesired() * invertFactor * powerLimitScalar);
}
}  // namespace motors